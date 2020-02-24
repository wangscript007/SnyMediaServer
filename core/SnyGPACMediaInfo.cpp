#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include "SnyGPACMediaInfo.h"
#include "SnyConstants.h"

using namespace rapidjson;

GPACAudioTrackInfo::GPACAudioTrackInfo(SNY_UI32 trackid, SNY_UI32 tracknumber, SNY_SI64 duration, std::string type, std::string codec, std::string lang) {
  this->mTrackID = trackid;
  this->mTrackNumber = tracknumber;
  this->mTrackDuration = duration;
  this->mTrackType = type;
  this->mTrackCodec = codec;
  this->mTrackLanguage = lang;
}

GPACAudioTrackInfo::~GPACAudioTrackInfo() {
  
}

GPACAudioTrackInfo::GPACAudioTrackInfo(const GPACAudioTrackInfo &object) {
  operator = (object);
}

void GPACAudioTrackInfo::operator=(const GPACAudioTrackInfo &object) {
  this->mTrackID = object.mTrackID;
  this->mTrackNumber = object.mTrackNumber;
  this->mTrackDuration = object.mTrackDuration;
  this->mTrackType = object.mTrackType;
  this->mTrackCodec = object.mTrackCodec;
  this->mTrackLanguage = object.mTrackLanguage;
}

GPACVideoTrackInfo::GPACVideoTrackInfo(SNY_UI32 trackid, SNY_UI32 tracknumber, SNY_SI64 duration, std::string type, std::string codec) {
  this->mTrackID = trackid;
  this->mTrackNumber = tracknumber;
  this->mTrackDuration = duration;
  this->mTrackType = type;
  this->mTrackCodec = codec;
}

GPACVideoTrackInfo::~GPACVideoTrackInfo() {
  
}

GPACVideoTrackInfo::GPACVideoTrackInfo(const GPACVideoTrackInfo &object) {
  operator=(object);
}

void GPACVideoTrackInfo::operator=(const GPACVideoTrackInfo &object) {
  this->mTrackID = object.mTrackID;
  this->mTrackNumber = object.mTrackNumber;
  this->mTrackDuration = object.mTrackDuration;
  this->mTrackType = object.mTrackType;
  this->mTrackCodec = object.mTrackCodec;
}

GPACMediaInfo::GPACMediaInfo() {
  this->mMediaFileName = "";
  this->mMediaFilePath = "";
  this->mMediaFileDuration = 0;
  this->mMediaFileSize = 0;
  this->mMediaFileErrorMsg = "";
}

GPACMediaInfo::GPACMediaInfo(std::string name, std::string path, SNY_UI64 duration, SNY_UI64 size, std::string errorMsg) {
  this->mMediaFileName = name;
  this->mMediaFilePath = path;
  this->mMediaFileDuration = duration;
  this->mMediaFileSize = size;
  this->mMediaFileErrorMsg = errorMsg;
}

GPACMediaInfo::~GPACMediaInfo() { 
  while (!mAudioTrackInfos.empty()) {
    GPACAudioTrackInfo *audio = mAudioTrackInfos.back();
    delete audio; mAudioTrackInfos.pop_back();
  }
  while (!mVideoTrackInfos.empty()) {
    GPACVideoTrackInfo *video = mVideoTrackInfos.back();
    delete video; mVideoTrackInfos.pop_back();
  }
}

GPACMediaInfo::GPACMediaInfo(const GPACMediaInfo &object) {
  operator=(object);
}

void GPACMediaInfo::operator=(const GPACMediaInfo &object) {
  this->mMediaFileName = object.mMediaFileName;
  this->mMediaFilePath = object.mMediaFilePath;
  this->mMediaFileDuration = object.mMediaFileDuration;
  this->mMediaFileSize = object.mMediaFileSize;
  this->mMediaFileErrorMsg = object.mMediaFileErrorMsg;
  
  for (auto iter = object.mAudioTrackInfos.begin(); iter != object.mAudioTrackInfos.end(); iter++) {
    auto trackInfo = new GPACAudioTrackInfo(**iter);
    this->mAudioTrackInfos.push_back(trackInfo);
  }
  
  for (auto iter = object.mVideoTrackInfos.begin(); iter != object.mVideoTrackInfos.end(); iter++) {
    auto trackInfo = new GPACVideoTrackInfo(**iter);
    this->mVideoTrackInfos.push_back(trackInfo);
  }
}

void GPACMediaInfo::addAudioTrackInfo(GPACAudioTrackInfo *audiotrackinfo) {
  mAudioTrackInfos.push_back(audiotrackinfo);
}

void GPACMediaInfo::addVideoTrackInfo(GPACVideoTrackInfo *videotrackinfo) { 
  mVideoTrackInfos.push_back(videotrackinfo);
}

std::string GPACMediaInfo::toJSON() {
  StringBuffer sb;
  Writer<StringBuffer> writer(sb);
  writer.StartObject();
  writer.Key(MEDIAINFONAME.c_str()); writer.String(mMediaFileName.c_str());
  writer.Key(MEDIAINFOPATH.c_str()); writer.String(mMediaFilePath.c_str());
  writer.Key(MEDIAINFODURATION.c_str()); writer.Uint64(mMediaFileDuration);
  writer.Key(MEDIAINFOSIZE.c_str()); writer.Uint64(mMediaFileSize);
  writer.Key(MEDIAINFOERROR.c_str()); writer.String(mMediaFileErrorMsg.c_str());
  
  writer.Key(MEDIAINFOTRACKS.c_str());
  writer.StartArray();
  size_t audioTrackCnt = mAudioTrackInfos.size();
  for (size_t i = 0; i < audioTrackCnt; i++) {
    writer.StartObject();
    writer.Key(MEDIAINFOTRACKID.c_str()); writer.Uint(mAudioTrackInfos[i]->mTrackID);
    writer.Key(MEDIAINFOTRACKNUMBER.c_str()); writer.Uint(mAudioTrackInfos[i]->mTrackNumber);
    writer.Key(MEDIAINFOTRACKTYPE.c_str()); writer.String(mAudioTrackInfos[i]->mTrackType.c_str());
    writer.Key(MEDIAINFOTRACKCODEC.c_str()); writer.String(mAudioTrackInfos[i]->mTrackCodec.c_str());
    writer.Key(MEDIAINFOTRACKLANGUAGE.c_str()); writer.String(mAudioTrackInfos[i]->mTrackLanguage.c_str());
    writer.Key(MEDIAINFODURATION.c_str()); writer.Uint64(mAudioTrackInfos[i]->mTrackDuration);
    writer.EndObject();
  }
  size_t videoTrackCnt = mVideoTrackInfos.size();
  for (size_t i = 0; i < videoTrackCnt; i++) {
    writer.StartObject();
    writer.Key(MEDIAINFOTRACKID.c_str()); writer.Uint(mVideoTrackInfos[i]->mTrackID);
    writer.Key(MEDIAINFOTRACKNUMBER.c_str()); writer.Uint(mVideoTrackInfos[i]->mTrackNumber);
    writer.Key(MEDIAINFOTRACKTYPE.c_str()); writer.String(mVideoTrackInfos[i]->mTrackType.c_str());
    writer.Key(MEDIAINFOTRACKCODEC.c_str()); writer.String(mVideoTrackInfos[i]->mTrackCodec.c_str());
    writer.Key(MEDIAINFODURATION.c_str()); writer.Uint64(mVideoTrackInfos[i]->mTrackDuration);
    writer.EndObject();
  }
  writer.EndArray();
  
  writer.EndObject();
  std::string JSON = sb.GetString();
  return JSON;
}
