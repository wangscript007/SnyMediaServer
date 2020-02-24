#include "SnyVODMediaManifest.h"
#include "SnyHLSConstants.h"
#include "SnyConstants.h"
#include "SnyUtils.h"

SNYVODMediaManifest::~SNYVODMediaManifest() { 
  return;
}

SNYVODMediaManifest::SNYVODMediaManifest() {
  this->channelID = "";
  this->duration = 0;;
}

void SNYVODMediaManifest::setChannelID(std::string channelID) { 
  this->channelID = channelID;
}

void SNYVODMediaManifest::setMediaDuration(SNY_SI64 duration) { 
  this->duration = duration;
}

SnyDataBuffer SNYVODMediaManifest::getMediaManifest() {
  SnyDataBuffer buffer;
  if (channelID.empty() || duration <= 0) {
    return buffer;
  }
  
  buffer.append(HLS_IDENTIFIER_TAG.data(), static_cast<int>(HLS_IDENTIFIER_TAG.size()));
  buffer.append(SNY_NEWLINE_TAG.data(), static_cast<int>(SNY_NEWLINE_TAG.size()));
  
  std::string target_duration = HLS_TARGET_DURATION_TAG + ":" + std::to_string(SNY_HLS_AV_SEGMENT_DURATION_SECOND) + SNY_NEWLINE_TAG;
  buffer.append(target_duration.data(), static_cast<int>(target_duration.size()));
  
  std::string media_sequence = HLS_MEDIA_SEQUENCE_TAG + ":" + std::to_string(0) + SNY_NEWLINE_TAG;
  buffer.append(media_sequence.data(), static_cast<int>(media_sequence.size()));
  
  std::string extinf = HLS_EXTINF_TAG + ":" + std::to_string(SNY_HLS_AV_SEGMENT_DURATION_SECOND) + "," + SNY_NEWLINE_TAG;
  
  SNY_SI64 total_sequence = duration/SNY_HLS_AV_SEGMENT_DURATION_SECOND + ( (duration%SNY_HLS_AV_SEGMENT_DURATION_SECOND)==0 ? 0 : 1 );
  for (SNY_SI64 i = 0; i < total_sequence; i++) {
    if (i < (total_sequence - 1)) {
      buffer.append(extinf.data(), static_cast<int>(extinf.size()));
    } else {
      SNY_SI64 last_segment_duration = duration%SNY_HLS_AV_SEGMENT_DURATION_SECOND;
      std::string last_extinf = HLS_EXTINF_TAG + ":" + std::to_string(last_segment_duration) + "," + SNY_NEWLINE_TAG;
      buffer.append(last_extinf.data(), static_cast<int>(last_extinf.size()));
    }
    std::string media_file_name = std::to_string(i) + ".ts" + SNY_NEWLINE_TAG;
    buffer.append(media_file_name.data(), static_cast<int>(media_file_name.size()));
  }

  std::string endlist = HLS_ENDLIST_TAG + SNY_NEWLINE_TAG;
  buffer.append(endlist.data(), static_cast<int>(endlist.size()));
  
  return buffer;
}

std::string SNYVODMediaManifest::getManifestUri() {
  std::string uri = "";
  if (channelID.empty()) {
    return uri;
  }
  uri = channelID + "/playlist.m3u8";
  return uri;
}

