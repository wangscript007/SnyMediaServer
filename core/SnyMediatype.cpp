#include "SnyMediatype.h"

std::string StreamType2String(StreamType type)
{
    switch(type)
    {
    case VideoStream:       return "VideoStream";
    case AudioStream:       return "AudioStream";
    case TextStream:        return "TextStream";
    case DataStream:        return "DataStream";
    case CCStream:          return "CCStream";
    case WebVttStream:      return "WebVttStream";
    case SubtitleStream:    return "SubtitleStream";
    default: break ;
    }
    return "UnknownStream";
}

std::string CodecType2String(CodecType type)
{
    switch(type)
    {
    case H264Codec:         return "H264Codec";
    case VP8Codec:          return "VP8Codec";

    case AACCodec:          return "AACCodec";
    case MP3Codec:          return "MP3Codec";

    case TX3GCodec:         return "TX3GCodec";

    case CC608TextCodec:    return "CC608TextCodec";
    case CC608RawCodec:     return "CC608RawCodec";

    default: break ;
    }
    return "UnknownCodec";
}
