#include "SnyResults.h"
#include "SnyGPACMP4.h"
#include "SnyConfig.h"
#include "easylogging++.h"
#include "SnyConstants.h"
#include "SnyGPACMediaInfo.h"
#include "SnyUtils.h"
#include <errno.h>
extern "C" {
#include <gpac/setup.h>
}
SNYGPACMP4::SNYGPACMP4() {
  mGFISOFile = nullptr;
  return;
}

SNYGPACMP4::~SNYGPACMP4() {
  if (mGFISOFile != nullptr) {
    gf_isom_close(mGFISOFile);
    mGFISOFile = nullptr;
  }
  while (!mSelectedTracks.empty()) {
    auto iter = mSelectedTracks.begin();
    auto sny_gpac_mp4_track = iter->second;
    delete sny_gpac_mp4_track;
    mSelectedTracks.erase(iter);
  }
  return;
}

void SNYGPACMP4::setUri(std::string uri) {
  mURI = uri;
  LOG(TRACE) << mURI;
}

SNY_Result SNYGPACMP4::open() {
#if 0
  FILE* file = SNY_fopen(mURI.c_str(), "r");
  if (file == nullptr) {
    char *e = strerror(errno);
    LOG(TRACE) << e;
    return SNY_FAILURE;
  }
#endif
  if((mGFISOFile = gf_isom_open(mURI.c_str(), GF_ISOM_OPEN_READ, nullptr)) == nullptr){
    LOG(ERROR) << "Failed to open " << mURI;
    return SNY_FAILURE;
  }
  return SNY_SUCCESS;
}

GPACMediaInfo SNYGPACMP4::getMediaInfo() {
  if (mGFISOFile == nullptr) {
    GPACMediaInfo mediainfo(mURI, mURI, 0, 0, "");
    return mediainfo;
  }
  u32 file_timescale = gf_isom_get_timescale(mGFISOFile);
  u64 file_duration  = gf_isom_get_duration(mGFISOFile);
  u64 file_size      = gf_isom_get_file_size(mGFISOFile);
  u32 track_count    = gf_isom_get_track_count(mGFISOFile);
  
  u64 duration = 0;
  if (file_duration != 0 || file_timescale != 0) {
    duration = file_duration * 1000 / file_timescale;
  }
  
  GPACMediaInfo mediainfo(mURI, mURI, duration, file_size, "");
  
  for(u32 i = 0; i < track_count; i++) {
    const u32 track_number    = i + 1;
    const u32 track_id        = gf_isom_get_track_id(mGFISOFile, track_number);
    const u32 media_type      = gf_isom_get_media_type(mGFISOFile, track_number);
    const u32 media_subtype   = gf_isom_get_media_subtype(mGFISOFile, track_number, 1);
    const u32 mpeg4_subtype   = gf_isom_get_mpeg4_subtype(mGFISOFile, track_number, 1);
    const u64 media_duration  = gf_isom_get_media_duration(mGFISOFile, track_number);
    const u64 media_timescale = gf_isom_get_media_timescale(mGFISOFile, track_number);
    duration = 0;
    if (media_duration != 0 && media_timescale != 0) {
      duration = media_duration * 1000 / media_timescale;
    }
    if (media_type == GF_ISOM_MEDIA_AUDIO && mpeg4_subtype == GF_4CC( 'm', 'p', '4', 'a' )) {
      char *lang = nullptr;
      GF_Err e = gf_isom_get_media_language(mGFISOFile, track_number, &lang);
      if (e != GF_OK) {
        lang = const_cast<char *>(sny_langauge_undefine);
      }
      std::string type  = std::string(sny_track_type_audio);
      std::string codec = std::string(sny_audio_codec_aac);
      std::string lan   = std::string(lang);
      GPACAudioTrackInfo *audio = new GPACAudioTrackInfo(track_id, track_number, duration, type, codec, lan);
      mediainfo.addAudioTrackInfo(audio);
    }
    if (media_type == GF_ISOM_MEDIA_VISUAL && media_subtype == GF_ISOM_SUBTYPE_AVC_H264) {
      std::string type  = std::string(sny_track_type_video);
      std::string codec = std::string(sny_video_codec_h264);
      GPACVideoTrackInfo *video = new GPACVideoTrackInfo(track_id, track_number, duration, type, codec);
      mediainfo.addVideoTrackInfo(video);
    }
  }
  return mediainfo;
}

SNY_Result SNYGPACMP4::selectTrack(int trackNumber) {
  if (mGFISOFile == nullptr) {
    return SNY_FAILURE;
  }
  
  std::map<int, SNYGPACMP4Track*>::iterator it = mSelectedTracks.find(trackNumber);
  if (it != mSelectedTracks.end()) {
    return SNY_SUCCESS;
  }
  
  u32 nalu_extract_mode = gf_isom_get_nalu_extract_mode(mGFISOFile, trackNumber);
  nalu_extract_mode = nalu_extract_mode | GF_ISOM_NALU_EXTRACT_TILE_ONLY | GF_ISOM_NALU_EXTRACT_ANNEXB_FLAG | GF_ISOM_NALU_EXTRACT_INBAND_PS_FLAG;
  GF_Err e = gf_isom_set_nalu_extract_mode(mGFISOFile, trackNumber, nalu_extract_mode);
  if (e != GF_OK) {
    return SNY_FAILURE;
  }
  
  SNY_Bool isValidTrack = false;
  const u32 track_id        = gf_isom_get_track_id(mGFISOFile, trackNumber);
  const u32 media_type      = gf_isom_get_media_type(mGFISOFile, trackNumber);
  const u32 media_subtype   = gf_isom_get_media_subtype(mGFISOFile, trackNumber, 1);
  const u32 mpeg4_subtype   = gf_isom_get_mpeg4_subtype(mGFISOFile, trackNumber, 1);
  const u32 media_timescale = gf_isom_get_media_timescale(mGFISOFile, trackNumber);
  const u32 sample_count    = gf_isom_get_sample_count(mGFISOFile, trackNumber);
  SNY_TRACKTYPE type = SNY_UNSUPPORT; GF_DecoderConfig *dcfg = NULL;
  if (media_type == GF_ISOM_MEDIA_AUDIO && mpeg4_subtype == GF_4CC( 'm', 'p', '4', 'a' )) {
    isValidTrack = true;
    type = SNY_AUDIO;
    dcfg = gf_isom_get_decoder_config(mGFISOFile, trackNumber, 1);
  }
  if (media_type == GF_ISOM_MEDIA_VISUAL && media_subtype == GF_ISOM_SUBTYPE_AVC_H264) {
    isValidTrack = true;
    type = SNY_VIDEO;
  }
  if (isValidTrack) {
    SNYGPACMP4Track *track = new SNYGPACMP4Track(type, track_id, trackNumber, dcfg, media_timescale, sample_count);
    SNY_Result result = track->parse();
    if (result != SNY_SUCCESS) {
      delete track; track = nullptr;
      return result;
    }
    mSelectedTracks.insert(std::make_pair(trackNumber, track));
  }
  return SNY_SUCCESS;
}

SNY_Result SNYGPACMP4::seek(SNY_UI64 position_us) {
  std::map<int, SNYGPACMP4Track*>::iterator it = mSelectedTracks.begin();
  for (; it != mSelectedTracks.end(); it++) {
    SNY_Result result = seek(it->first, position_us);
    if (result != SNY_SUCCESS) {
      return SNY_FAILURE;
    }
  }
  return SNY_SUCCESS;
}

SNY_Result SNYGPACMP4::seek(int trackNumber, SNY_UI64 position_us) {
  if (mGFISOFile == nullptr) {
    LOG(ERROR) << "File not Opened";
    return SNY_FAILURE;
  }
  
  if (mSelectedTracks.find(trackNumber) == mSelectedTracks.end()) {
    return SNY_FAILURE;
  }
  
  const u32 media_timescale = gf_isom_get_media_timescale(mGFISOFile, trackNumber);
  u64 desiredTime = SNYUtils::convertTime(position_us, SNY_TIMESCALE_MICROSECOND, media_timescale);
  u32 streamDescIndex; GF_ISOSample * sample = nullptr; u32 num;
  GF_Err e = gf_isom_get_sample_for_media_time(mGFISOFile, trackNumber, desiredTime, &streamDescIndex, GF_ISOM_SEARCH_FORWARD, &sample, &num);
  if (e == GF_EOS) {
    mSelectedTracks[trackNumber]->end_of_track = true;
    return SNY_ERROR_EOS;
  }
  mSelectedTracks[trackNumber]->next_sample_number = num;
  mSelectedTracks[trackNumber]->end_of_track = false;
  return SNY_SUCCESS;
}

SNY_Bool SNYGPACMP4::reachEndOfFile() {
  return false;
}

SNY_Bool SNYGPACMP4::reachEndOfTrack(int trackNumber) {
  if (mSelectedTracks.find(trackNumber) == mSelectedTracks.end()) {
    return true;
  }
  return mSelectedTracks[trackNumber]->reachEnd();
}

SnySample *SNYGPACMP4::readSample(int trackNumber) {
  SnySample *sample = nullptr;
  if (mGFISOFile == nullptr) {
    LOG(ERROR) << "File not Opened";
    return sample;
  }
  
  if (mSelectedTracks.find(trackNumber) == mSelectedTracks.end()) {
    LOG(ERROR) << "Track " << trackNumber << "is Not Selected";
    return sample;
  }
  
  SNYGPACMP4Track *track = mSelectedTracks[trackNumber];
  GF_ISOSample *iso_sample;
  u32 sample_description_index;
  iso_sample = gf_isom_get_sample(mGFISOFile, trackNumber, track->next_sample_number, &sample_description_index);
  track->next_sample_number++;
  if (iso_sample != nullptr) {
    SNY_Bool sync = iso_sample->IsRAP;
    SNY_UI64 dts_us = SNYUtils::convertTime(iso_sample->DTS, track->time_scale, SNY_TIMESCALE_MICROSECOND);
    SNY_UI64 pts_us = SNYUtils::convertTime(iso_sample->DTS + iso_sample->CTS_Offset, track->time_scale, SNY_TIMESCALE_MICROSECOND);
    const char *iso_sample_data = iso_sample->data;
    const u32  iso_sample_size  = iso_sample->dataLength;
    sample = new SnySample();
    sample->setDts(dts_us);
    sample->setPts(pts_us);
    sample->setKey(sync);
    
    if (track->type == SNY_AUDIO) {
      sample->setType(AudioStream);
      /*assemble ADTS header*/
      GF_BitStream *bs;
      bs = gf_bs_new(nullptr, 0, GF_BITSTREAM_WRITE);
      gf_bs_write_int(bs, 0xFFF, 12);/*sync*/
      gf_bs_write_int(bs, (track->aac_mode==1) ? 1 : 0, 1);/*mpeg2 aac*/
      gf_bs_write_int(bs, 0, 2); /*layer*/
      gf_bs_write_int(bs, 1, 1); /* protection_absent*/
      gf_bs_write_int(bs, track->aac_type, 2);
      gf_bs_write_int(bs, track->a_cfg.base_sr_index, 4);
      gf_bs_write_int(bs, 0, 1);
      gf_bs_write_int(bs, track->a_cfg.nb_chan, 3);
      gf_bs_write_int(bs, 0, 4);
      gf_bs_write_int(bs, 7+iso_sample->dataLength, 13);
      gf_bs_write_int(bs, 0x7FF, 11);
      gf_bs_write_int(bs, 0, 2);
      gf_bs_write_data(bs, iso_sample_data, iso_sample_size);
      
      char *data = nullptr; u32 size = 0;
      if (bs != nullptr) {
        gf_bs_get_content(bs, &data, &size);
        if (data != nullptr && size > 0) {
          sample->setData(data, size);
          gf_free(data);
        }
      }
      
      gf_bs_del(bs);
    } else if (track->type == SNY_VIDEO) {
      sample->setType(VideoStream);
      sample->setData(iso_sample_data, iso_sample_size);
    } else {
      delete sample; sample = nullptr;
      LOG(ERROR) << "";
    }
    
    gf_isom_sample_del(&iso_sample);
  } else {
    mSelectedTracks[trackNumber]->end_of_track = true;
    GF_Err e = gf_isom_last_error(mGFISOFile);
    const char* error = gf_error_to_string(e);
    LOG(ERROR) << error;
    return sample;
  }
  return sample;
}
