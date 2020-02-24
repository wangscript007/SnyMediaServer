#include "http_content_type.h"
#include "http_server.h"
#include "http_headers.h"
#include "SnyConstants.h"
#include "Ap4Mpeg2Ts.h"
#include "Ap4ByteStream.h"
#include "Ap4Sample.h"
#include "Ap4DataBuffer.h"
#include "easylogging++.h"
#include "SnyVODChannel.h"
#include "SnyVODMediaManifest.h"
using namespace Network;

SNYVODChannel::SNYVODChannel(std::string channelID) {
  this->channelID = channelID;
  this->uri = "";
  this->manifest = nullptr;
  this->demuxer = nullptr;
  this->currentSegmentIndex = 0;
  return;
}

void SNYVODChannel::release() {
  if (demuxer) {
    delete demuxer; demuxer = nullptr;
  }
  if (manifest != nullptr) {
    delete manifest; manifest = nullptr;
  }
  while (!samples.empty()) {
    auto sample = samples.front();
    samples.pop_front();
    delete sample;
  }
  std::list<SnySample*>(samples).swap(samples);
  return;
}

void SNYVODChannel::requestContent(Network::IHttpRequestPtr req) {
  std::string path = req->GetPath();
  if (path.find(".m3u8") != std::string::npos) {
    internalProcessMediaManifestRequest(req);
  } else if (path.find(".ts") != std::string::npos){
    if (demuxer == nullptr) {
      internalSegmentNotFound(req);
    } else {
      internalProcessSegmentRequest(req);
    }
  } else {
    internalSegmentNotFound(req);
  }
  return;
}


void SNYVODChannel::setUri(std::string uri) {
  this->uri = uri;
  if (demuxer != nullptr) {
    return;
  }
  internalMediaOpen();
}

std::string SNYVODChannel::getChannelID() { 
  return this->channelID;
}

SNYVODChannel::~SNYVODChannel() {
  release();
  return;
}

SNY_Bool SNYVODChannel::getPlaybackUri(std::string host, SNYPlaylists& playlists) {
  if (manifest != nullptr) {
    std::string protocol = "hls";
    std::string playbackuri = SNY_HTTP_C + host + SNY_FORWARDSLASH_C + \
    SNY_MEDIASERVER_MEDIA_C + SNY_FORWARDSLASH_C + \
    SNY_MEDIASERVER_VOD_C + SNY_FORWARDSLASH_C + \
    manifest->getManifestUri();
    SNYPlaylist pl(protocol, playbackuri);
    playlists.push(pl);
    return true;
  };
  return false;
}

SNY_Bool SNYVODChannel::internalMediaOpen() {
  if (demuxer != nullptr) {
    return true;
  }
  
  demuxer = SNYIDemuxer::createInstance();
  if (demuxer == nullptr) {
    return false;
  }
  demuxer->setUri(this->uri);
  SNY_Result result = demuxer->open();
  if (SNYFAILED(result)) {
    delete demuxer; demuxer = nullptr;
    return false;
  }
  
  mediaInfo = demuxer->getMediaInfo();
  
  for (auto iter = mediaInfo.mAudioTrackInfos.begin(); iter != mediaInfo.mAudioTrackInfos.end(); iter++) {
    if ((**iter).mTrackType.compare(sny_track_type_audio) == 0) {
      SNY_UI32 trackNumber = (**iter).mTrackNumber;
      demuxer->selectTrack(trackNumber);
      selectedTrackNumbers.push_back(trackNumber);
      break;
    }
  }
  for (auto iter = mediaInfo.mVideoTrackInfos.begin(); iter != mediaInfo.mVideoTrackInfos.end(); iter++) {
    if ((**iter).mTrackType.compare(sny_track_type_video) == 0) {
      SNY_UI32 trackNumber = (**iter).mTrackNumber;
      demuxer->selectTrack(trackNumber);
      selectedTrackNumbers.push_back(trackNumber);
      break;
    }
  }
  internalMediaManifest();
  return true;
}

SNY_Bool SNYVODChannel::internalMediaSeekTo(SNY_SI64 position) {
  demuxer->seek(position);
  for (auto iter = selectedTrackNumbers.begin(); iter != selectedTrackNumbers.end(); iter++) {
    SNY_UI32 trackNumber = *iter;
    while (!demuxer->reachEndOfTrack(trackNumber)) {
      SnySample* sample = demuxer->readSample(trackNumber);
      if (sample != nullptr) {
        if (sample->dts() >= position) {
          samples.push_back(sample);
          break;
        } else {
          delete sample; sample = nullptr;
        }
      } else {
        break;
      }
    }
  }
  return true;
}

SNY_Bool SNYVODChannel::internalMediaReadTo(SNY_SI64 position) {
  for (auto iter = selectedTrackNumbers.begin(); iter != selectedTrackNumbers.end(); iter++) {
    SNY_UI32 trackNumber = *iter;
    while (!demuxer->reachEndOfTrack(trackNumber)) {
      SnySample* sample = demuxer->readSample(trackNumber);
      if (sample != nullptr) {
        samples.push_back(sample);
        if (sample->dts() >= position) {
          break;
        }
      } else {
        break;
      }
    }
  }

  samples.sort([](const auto first, const auto second) {
    if (first->dts() < second->dts()) {
      return true;
    }
    return false;
  });

  return true;
}

SNY_Bool SNYVODChannel::internalMuxSegment(SNY_SI64 position, Network::IHttpRequestPtr req) {
  static unsigned int pmt_pid           = AP4_MPEG2_TS_DEFAULT_PID_PMT;
  static unsigned int video_pid         = AP4_MPEG2_TS_DEFAULT_PID_VIDEO;
  static unsigned int audio_pid         = AP4_MPEG2_TS_DEFAULT_PID_AUDIO;
  static unsigned int audio_stream_type = AP4_MPEG2_STREAM_TYPE_ISO_IEC_13818_7;
  static unsigned int audio_stream_id   = AP4_MPEG2_TS_DEFAULT_STREAM_ID_AUDIO;
  static unsigned int video_stream_type = AP4_MPEG2_STREAM_TYPE_AVC;
  static unsigned int video_stream_id   = AP4_MPEG2_TS_DEFAULT_STREAM_ID_VIDEO;
  
  std::string path = req->GetPath();
  SNY_Bool success = false;
  AP4_Mpeg2TsWriter::SampleStream* audio_stream = NULL;
  AP4_Mpeg2TsWriter::SampleStream* video_stream = NULL;
  AP4_MemoryByteStream* memoryByteStream = new AP4_MemoryByteStream();
  AP4_Mpeg2TsWriter* mpeg2Writer         = new AP4_Mpeg2TsWriter(pmt_pid);
  do {
    if (samples.empty()) {
      break;
    }
    
    AP4_Result result;
    result = mpeg2Writer->SetAudioStream(SNY_TIMESCALE_MICROSECOND, audio_stream_type, audio_stream_id, audio_stream, audio_pid, NULL, 0);
    if (AP4_FAILED(result)) {
      LOG(ERROR) << "MPEG2 TS, failed to create video stream";
      break;
    }
    result = mpeg2Writer->SetVideoStream(SNY_TIMESCALE_MICROSECOND, video_stream_type, video_stream_id, video_stream, video_pid, NULL, 0);
    if (AP4_FAILED(result)) {
      LOG(ERROR) << "MPEG2 TS, failed to create audio stream";
      break;
    }
    mpeg2Writer->WritePAT(*memoryByteStream);
    mpeg2Writer->WritePMT(*memoryByteStream);
    success = true;
    while (!samples.empty()) {
      SnySample* sample = samples.front();
      if (sample->dts() < position) {
        samples.pop_front();
        AP4_Sample Ap4Sample;
        Ap4Sample.SetDts(sample->dts());
        Ap4Sample.SetCts(sample->pts());
        Ap4Sample.SetSync(sample->isKey());
        AP4_DataBuffer sample_data(sample->data(), sample->size());
        if (sample->type() == AudioStream) {
          result = audio_stream->WriteSample(Ap4Sample, sample_data, NULL, true, *memoryByteStream);
        } else if (sample->type() == VideoStream) {
          result = video_stream->WriteSample(Ap4Sample, sample_data, NULL, true, *memoryByteStream);
        } else {
          
        }
        delete sample; sample = nullptr;
        if (AP4_FAILED(result)){
          break;
        }
      } else {
        break;
      }
    }
  } while (false);
  
  if (success) {
    mpeg2Writer->WriteMPEG2PacketCCTO16(*memoryByteStream);
    internalSaveDataToRequest(req, memoryByteStream->GetData(), static_cast<SNY_SI32>(memoryByteStream->GetDataSize()));
  } else {
    internalSegmentNotFound(req);
  }
  currentSegmentIndex++;
  
  if (memoryByteStream != nullptr) {
    memoryByteStream->Release();memoryByteStream = nullptr;
  }
  
  if (mpeg2Writer != nullptr) {
    delete mpeg2Writer; mpeg2Writer = nullptr;
  }
  return true;
}

void SNYVODChannel::internalSegmentNotFound(Network::IHttpRequestPtr req) {
  req->SetResponseNotFound();
}

void SNYVODChannel::internalProcessMediaManifestRequest(Network::IHttpRequestPtr req) {
  SnyDataBuffer buffer;
  if (manifest != nullptr) {
    buffer = manifest->getMediaManifest();
  }
  internalSaveDataToRequest(req, reinterpret_cast<const SNY_UI08*>(buffer.data()), buffer.size());
}

void SNYVODChannel::internalProcessSegmentRequest(Network::IHttpRequestPtr req) {
  std::string path = req->GetPath();
  std::string fileName;
  std::string::size_type pos = path.find_last_of("/");
  if (pos != std::string::npos) {
    fileName = path.substr(pos+1, path.length() - 4);
    SNY_SI64 index = std::stol(fileName);
    
    SNY_SI64 segment_min_dts = index * 10 * SNY_TIMESCALE_MICROSECOND;
    SNY_SI64 segment_max_dts = (index + 1) * 10 * SNY_TIMESCALE_MICROSECOND;
    
    if (index != currentSegmentIndex) {
      while (!samples.empty()) {
        SnySample* sample = samples.front();
        samples.pop_front();
        delete sample; sample = nullptr;
      }
      std::list<SnySample*>(samples).swap(samples);
      internalMediaSeekTo(segment_min_dts);
      currentSegmentIndex = index;
    }
    internalMediaReadTo(segment_max_dts);
    internalMuxSegment(segment_max_dts, req);
  }
}

void SNYVODChannel::internalSaveDataToRequest(Network::IHttpRequestPtr req, const SNY_UI08* data, const SNY_SI32 size) {
  req->SetResponseBuf(data, size);
}

void SNYVODChannel::internalMediaManifest() {
  if (manifest != nullptr) {
    return;
  }
  manifest = new SNYVODMediaManifest();
  manifest->setChannelID(channelID);
  SNY_SI64 duration_second = 0;
  if (mediaInfo.mMediaFileDuration > 0) {
    duration_second = mediaInfo.mMediaFileDuration/SNY_TIMESCALE_MILLISECOND;
  }
  manifest->setMediaDuration(duration_second);
}
