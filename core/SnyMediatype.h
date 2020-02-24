#ifndef _STREAMINGNOLOGY_CORE_MEDIATYPE_H_
#define _STREAMINGNOLOGY_CORE_MEDIATYPE_H_

#include "typedefine.h"
#include "SnyDataBuffer.h"
#include <string>
#include <vector>
#include <list>
#include <stdlib.h>
#include <string.h>

typedef struct _VideoStreamFormat
{
	int         width;
	int         height;
	double      frameRate;
	int         pixelFormat;

	void reset()
	{
		width		= 0;
		height		= 0;
		frameRate	= 0.0;
		pixelFormat = 0;
	}
	
}VideoStreamFormat;

typedef struct _AudioStreamFormat
{
	int    channels;
	int    bitsPerSample;
	int    sampleRate;
	int    sampleFormat;
    char   name[256];
	char   language[256];

	void reset()
	{
		channels = 0;
		bitsPerSample = 0;
		sampleRate = 0;
        sampleFormat = -1;
		memset(name,0, sizeof(name));
		memset(language, 0, sizeof(language));
	}
}AudioStreamFormat;

typedef struct _TextStreamFormat
{
	char language[5];
	int  type;

	void reset()
	{
		memset(language,0, 5*sizeof(char));
		type = -1;
	}
}TextStreamFormat;

typedef struct _CCStreamFormat
{
	int  channel;
    char language[5];

	void reset()
	{
		channel = -1;
        memset(language,0, 5*sizeof(char));
	}
}CCStreamFormat;

typedef struct _SubtitleStreamFormat
{
    char   name[256];
    char   language[256];

    void reset()
    {
        memset(name, 0, sizeof(name));
        memset(language, 0, sizeof(language));
    }
}SubtitleStreamFormat;

typedef struct _ThumbnailStreamFormat
{
	int         width;
	int         height;
	long		bitrate;

	void reset()
	{
		width = 0;
		height = 0;
		bitrate = 0;
	}
}ThumbnailStreamFormat;

enum StreamType
{
	UnknownStream = 0,
	VideoStream,
	AudioStream,
	TextStream,
    DataStream,
    CCStream,
	WebVttStream,
    SubtitleStream,
	ThumbnailStream,
};

std::string StreamType2String(StreamType type);

enum CodecType
{
	UnknownCodec        = 0,
    
	VideoCodecBegin     = 1,     // Video Stream
	H264Codec           = 2,
	VP8Codec            = 3,
    HEVCCodec           = 4,

	AudioCodecBegin     = 10001, // Audio Stream
	AACCodec            = 10002,
	MP3Codec            = 10003,
	AC3Codec            = 10004,

    SubtitleCodecBegin  = 20001, // Subtitle Stream
    TX3GCodec           = 20002,
    WebVttTxtCodec      = 20003,
    ASSCodec            = 20004,
    BITMAPCodec         = 20005,
    TEXTCodec           = 20006,
    SRTCodec            = 20007,

    DataCodecBegin      = 30001, // Data Stream
	ID3Codec		    = 30002,
	M3U8TagCodec        = 30003,

    CCCodecBegin        = 40001, // CC Stream
    CC608TextCodec      = 40002, // CC608 Formatted Text
    CC608RawCodec       = 40003, // CC608 Raw Data, Pair Bytes

    // ffmpeg AVCodecID, StreamFormat::codecId
    FFMPEGCodecID       = 65535
};

std::string CodecType2String(CodecType type);

typedef struct _StreamFormat
{
	int        id;
	StreamType streamType;
	CodecType  codecType;
    int        codecId; // ffmpeg AVCodecID
	bool       selected;

    char *     privateData;
    int        privateSize;

    union
	{
		VideoStreamFormat       video;
		AudioStreamFormat       audio;
		TextStreamFormat        text;
		CCStreamFormat          cc;
        SubtitleStreamFormat    webVTT;
        SubtitleStreamFormat    subtitle;
		ThumbnailStreamFormat	thumbnail;
	};

    void setPrivateData(const char *data, int size)
    {
        if(privateData != NULL)
        {
            free(privateData);
            privateData = NULL;
        }
        privateSize = 0;
        if((data != NULL) && (size > 0))
        {
            privateData = (char *)malloc(size);
            if(privateData != NULL)
            {
                privateSize = size;
                memcpy(privateData, data, size);
            }
        }
    }

	void reset()
	{
		id          = -1;
        streamType  = UnknownStream;
        codecType   = UnknownCodec;
        codecId     = 0; // AV_CODEC_ID_NONE
		selected    = false;
	}

    _StreamFormat()
    {
        privateData = NULL;
        privateSize = 0;

        reset();
    }

    ~_StreamFormat()
    {
        if(privateData != NULL)
        {
            free(privateData);
            privateData = NULL;
        }
    }

    _StreamFormat(const _StreamFormat &obj)
    {
        privateData = NULL;
        privateSize = 0;
        reset();

        id              = obj.id;
        streamType      = obj.streamType;
        codecType       = obj.codecType;
        codecId         = obj.codecId;
        selected        = obj.selected;
        setPrivateData(obj.privateData, obj.privateSize);
        switch(streamType)
        {
        case VideoStream:    video    = obj.video;     break ;
        case AudioStream:    audio    = obj.audio;     break ;
        case TextStream:     text     = obj.text;      break ;
        case CCStream:       cc       = obj.cc;        break ;
        case WebVttStream:   webVTT   = obj.webVTT;    break ;
        case SubtitleStream: subtitle = obj.subtitle;  break ;
		case ThumbnailStream: thumbnail = obj.thumbnail;  break ;
        default: break ;
        }
    }

    void operator = (const _StreamFormat &o)
    {
        id              = o.id;
        streamType      = o.streamType;
        codecType       = o.codecType;
        codecId         = o.codecId;
        selected        = o.selected;
        setPrivateData(o.privateData, o.privateSize);
        switch(streamType)
        {
        case VideoStream:    video    = o.video;    break ;
        case AudioStream:    audio    = o.audio;    break ;
        case TextStream:     text     = o.text;     break ;
        case CCStream:       cc       = o.cc;       break ;
        case WebVttStream:   webVTT   = o.webVTT;   break ;
        case SubtitleStream: subtitle = o.subtitle; break ;
		case ThumbnailStream: thumbnail = o.thumbnail;  break ;
        default: break ;
        }
    }
}StreamFormat;

enum SampleType
{
	SampleNone          = 0x00000000,
	SampleData          = 0x00000001 << 0,
	SampleBegin         = 0x00000001 << 1,
	SampleFormatChanged = 0x00000001 << 2,
	SampleEnd           = 0x00000001 << 3,
	StreamChanged		= 0x00000001 << 4,
	SegmentEnd			= 0x00000001 << 5,

	CodecAbort          = 0x00000001 << 6,
	CodecEnded          = 0x00000001 << 7,
	CodecChanged        = 0x00000001 << 8,
	CodecDiscard        = 0x00000001 << 9,
};

#endif // _STREAMINGNOLOGY_CORE_MEDIATYPE_H_
