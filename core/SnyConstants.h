#ifndef _STREAMINGNOLOGY_CORE_SNYCONSTANTS_H_
#define _STREAMINGNOLOGY_CORE_SNYCONSTANTS_H_
#include <string>
#include "SnyType.h"

static const std::string COMPANYNAME = "streamingnology";

static const std::string MEDIAINFONAME = "name";
static const std::string MEDIAINFOPATH = "path";
static const std::string MEDIAINFODURATION = "duration";
static const std::string MEDIAINFOSIZE = "size";
static const std::string MEDIAINFOERROR = "error";
static const std::string MEDIAINFOTRACKS = "tracks";
static const std::string MEDIAINFOTRACKID = "trackid";
static const std::string MEDIAINFOTRACKNUMBER = "tracknumber";
static const std::string MEDIAINFOTRACKTYPE = "type";
static const std::string MEDIAINFOTRACKCODEC = "codec";
static const std::string MEDIAINFOTRACKLANGUAGE = "language";

static const char *sny_track_type_audio  = "audio";
static const char *sny_track_type_video  = "video";
static const char *sny_langauge_undefine = "und";
static const char *sny_audio_codec_aac   = "aac";
static const char *sny_video_codec_h264  = "h264";

static const SNY_SI32 SNY_SI32_MAX_VALUE = 0x7FFFFFFF;

static const SNY_UI64 SNY_TIMESCALE_MICROSECOND = 1000*1000;
static const SNY_UI64 SNY_TIMESCALE_MILLISECOND = 1000;

static const SNY_SI64 SNY_HLS_AV_SEGMENT_DURATION_SECOND = 10;
static const std::string   SNY_NEWLINE_TAG = "\n";

static const std::string SNY_MEDIASERVER_SYSTEM_C = "system";
static const std::string SNY_MEDIASERVER_MEDIA_C  = "media";
static const std::string SNY_MEDIASERVER_WEB_C    = "web";
static const std::string SNY_MEDIASERVER_VOD_C    = "vod";
static const std::string SNY_MEDIASERVER_LIVE_C   = "live";

static const std::string SNY_MEDIASERVER_SYSTEM_GETDIRECTORYCONTENT_C = "getDirectoryContent";
static const std::string SNY_MEDIASERVER_SYSTEM_GETSYSTEMINFO_C = "getSystemInfo";
static const std::string SNY_MEDIASERVER_MEDIA_GETMEDIAINFO_C = "getMediaInfo";

static const SNY_SI32    SNY_MEDIASERVER_ACCEPT_MAX_CONTENT_SIZE = 10 * 1024;//10K bytes

static const std::string SNY_MEDIASERVER_MEDIA_GETMEDIAINFO_PATH_C = "path";

static const std::string SNY_MEDIASERVER_PLAYLIST_PROTOCOL_C = "protocol";
static const std::string SNY_MEDIASERVER_PLAYLIST_URI_C = "uri";

static const std::string SNY_HTTP_C = "http://";
static const std::string SNY_FORWARDSLASH_C = "/";

enum SNYSERVICETYPE {
  SNY_SERVICE_UNKNOWN = 0,
  SNY_SERVICE_SYSTEM,
  SNY_SERVICE_MEDIA,
  SNY_SERVICE_WEB,

  SNY_SERVICE_SYSTEM_GETDIRECTORYCONTENT = 100,
  SNY_SERVICE_SYSTEM_GETSYSTEMINFO,
  
  SNY_SERVICE_MEDIA_GETMEDIAINFO         = 200,
  SNY_SERVICE_MEDIA_VOD,
  SNY_SERVICE_MEDIA_LIVE,
};

enum SNY_TRACKTYPE {
  SNY_UNSUPPORT = 0,
  SNY_AUDIO     = 1,
  SNY_VIDEO     = 2,
};

#endif /*_STREAMINGNOLOGY_CORE_SNYCONSTANTS_H_*/
