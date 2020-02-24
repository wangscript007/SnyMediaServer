#ifndef _STREAMINGNOLOGY_CORE_SNYGPACMEDIAINFO_H_
#define _STREAMINGNOLOGY_CORE_SNYGPACMEDIAINFO_H_
#include<string>
#include<vector>
#include"SNYType.h"

class GPACAudioTrackInfo {
public:
  GPACAudioTrackInfo(SNY_UI32 trackid, SNY_UI32 tracknumber, SNY_SI64 duration, std::string type, std::string codec, std::string lang);
  ~GPACAudioTrackInfo();
  
  GPACAudioTrackInfo(const GPACAudioTrackInfo &object);
  void operator = (const GPACAudioTrackInfo &object);
  SNY_UI32    mTrackID;
  SNY_UI32    mTrackNumber;
  SNY_UI64    mTrackDuration;
  std::string mTrackType;
  std::string mTrackCodec;
  std::string mTrackLanguage;
};

class GPACVideoTrackInfo {
public:
  GPACVideoTrackInfo(SNY_UI32 trackid, SNY_UI32 tracknumber, SNY_SI64 duration, std::string type, std::string codec);
  ~GPACVideoTrackInfo();

  GPACVideoTrackInfo(const GPACVideoTrackInfo &object);
  void operator = (const GPACVideoTrackInfo &object);
  SNY_UI32    mTrackID;
  SNY_UI32    mTrackNumber;
  SNY_UI64    mTrackDuration;
  std::string mTrackType;
  std::string mTrackCodec;
};
class GPACMediaInfo {
public:
  GPACMediaInfo();
  GPACMediaInfo(std::string name, std::string path, SNY_UI64 duration, SNY_UI64 size, std::string errorMsg);
  ~GPACMediaInfo();
  
  GPACMediaInfo(const GPACMediaInfo &object);
  void operator = (const GPACMediaInfo &object);
  
  void addAudioTrackInfo(GPACAudioTrackInfo *audiotrackinfo);
  void addVideoTrackInfo(GPACVideoTrackInfo *videotrackinfo);
  std::string toJSON();
  
  std::string                       mMediaFileName;
  std::string                       mMediaFilePath;
  SNY_UI64                          mMediaFileDuration;//SNY_TIMESCALE_MILLISECOND
  SNY_UI64                          mMediaFileSize;
  std::string                       mMediaFileErrorMsg;
  std::vector<GPACAudioTrackInfo*>  mAudioTrackInfos;
  std::vector<GPACVideoTrackInfo*>  mVideoTrackInfos;
};
#endif /*_STREAMINGNOLOGY_CORE_SNYGPACMEDIAINFO_H_*/
