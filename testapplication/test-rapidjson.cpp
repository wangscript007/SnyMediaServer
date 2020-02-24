#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <string>
#include "SNYGPACMediaInfo.h"
#include "easylogging++.h"
#include "SNYGPACMP4.h"
#include "Ap4ByteStream.h"
#include "Ap4Mpeg2Ts.h"
#include "Ap4Sample.h"
#include "Ap4DataBuffer.h"
#include "iostream.h"
using namespace rapidjson;
using namespace std;

void test_rapidjson() {
  std::string path = "/Users/neulion/Documents/docroot/media/现代世界的天才马克思.mp4";
  SNYGPACMP4 reader;
  reader.setUri(path);
  reader.open();
  GPACMediaInfo mediainfo = reader.getMediaInfo();
  LOG(TRACE) << mediainfo.toJSON();
  
  reader.selectTrack(1);
  reader.selectTrack(2);
  
#if 1
  do {
    Sample *sample = reader.readSample(1);
    if (sample != NULL) {
      LOG(TRACE) << "DTS is " << sample->dts() << " ,PTS is " << sample->pts() << " ,Size is " << sample->size() << " ,Sync " << (sample->isKey() ? "true" : "false") ;
      delete sample;
    }
  } while (!reader.reachEndOfTrack(1));
  
  //reader.seek(0);
  int jd = 0;
  jd ++;
  
  do {
    jd++;
    if (jd >= 300) {
      jd =0;
    }
    Sample *sample = reader.readSample(2);
    if (sample != NULL) {
      LOG(TRACE) << "DTS is " << sample->dts() << " ,PTS is " << sample->pts() << " ,Size is " << sample->size() << " ,Sync " << (sample->isKey() ? "true" : "false") ;
      delete sample;
    }
  } while (!reader.reachEndOfTrack(2));
  int i = 0;
  i ++;
  return;
#endif
#if 0
  string out = "/Users/neulion/Downloads/out.ts";
  unsigned int pmt_pid           = AP4_MPEG2_TS_DEFAULT_PID_PMT;
  unsigned int video_pid         = AP4_MPEG2_TS_DEFAULT_PID_VIDEO;
  unsigned int audio_pid         = AP4_MPEG2_TS_DEFAULT_PID_AUDIO;
  unsigned int audio_stream_type = AP4_MPEG2_STREAM_TYPE_ISO_IEC_13818_7;
  unsigned int audio_stream_id   = AP4_MPEG2_TS_DEFAULT_STREAM_ID_AUDIO;
  unsigned int video_stream_type = AP4_MPEG2_STREAM_TYPE_AVC;
  unsigned int video_stream_id   = AP4_MPEG2_TS_DEFAULT_STREAM_ID_VIDEO;
  
  AP4_Result result;
  
  AP4_Mpeg2TsWriter::SampleStream* audio_stream = NULL;
  AP4_Mpeg2TsWriter::SampleStream* video_stream = NULL;
  
  AP4_MemoryByteStream* memStream = new AP4_MemoryByteStream();
  AP4_Mpeg2TsWriter* writer = new AP4_Mpeg2TsWriter(pmt_pid);
  //result = writer->SetAudioStream(SNY_TIMESCALE_MICROSECOND, audio_stream_type, audio_stream_id, audio_stream, audio_pid, NULL, 0);
  if (AP4_FAILED(result)) {
    //fprintf(stderr, "could not create video stream (%d)\n", result);
    //return;
  }
  //result = writer->SetVideoStream(SNY_TIMESCALE_MICROSECOND, video_stream_type, video_stream_id, video_stream, video_pid, NULL, 0);
  if (AP4_FAILED(result)) {
    fprintf(stderr, "could not create video stream (%d)\n", result);
    //return;
  }
  
  writer->WritePAT(*memStream);
  writer->WritePMT(*memStream);
  Sample *vsample = nullptr;
  Sample *aSample = nullptr;
  int ij = 0;
  do {
    ij++;
    if (ij >= 300) {
      ij = 0;
    }
    bool writevideoSample = false;
    bool writeaudioSample = false;
    if (vsample == nullptr) {
      vsample = reader.readSample(1);
      delete vsample; vsample = nullptr;
    }
    if (aSample == nullptr) {
      aSample = reader.readSample(2);
      delete aSample; aSample = nullptr;
    }
    if (vsample != nullptr && aSample != nullptr) {
      if (vsample->dts() < aSample->dts()) {
        writevideoSample = true;
      } else {
        writeaudioSample = true;
      }
    } else if(vsample == nullptr && aSample != nullptr) {
      writeaudioSample = true;
    } else if(aSample == nullptr && vsample != nullptr) {
      writevideoSample = true;
    } else {
      //break;
    }
    
    if (writevideoSample && vsample != nullptr) {
      //AP4_Sample videoSample;
      //videoSample.SetDts(vsample->dts());
      //videoSample.SetCts(vsample->pts());
      //AP4_DataBuffer sample_data(vsample->data(), vsample->size());
      //result = video_stream->WriteSample(videoSample, sample_data, NULL, true, *memStream);
      delete vsample; vsample = nullptr;
      if (AP4_FAILED(result)){
        //break;
      }
    }
    if (writeaudioSample && aSample != nullptr) {
      //AP4_Sample videoSample;
      //videoSample.SetDts(aSample->dts());
      //videoSample.SetCts(aSample->pts());
      //AP4_DataBuffer sample_data(aSample->data(), aSample->size());
      //result = audio_stream->WriteSample(videoSample, sample_data, NULL, true, *memStream);
      delete aSample; aSample = nullptr;
      if (AP4_FAILED(result)){
        //break;
      }
    }
  } while (!reader.reachEndOfFile());
  
  FileIOStream *w = new FileIOStream(out.c_str(), FileIOStream::WriteOnly);
  w->write((char*)memStream->GetData(), memStream->GetDataSize());
  w->flush();
  delete w; w = nullptr;
  memStream->Release();
#endif
  
  int j = 0;
  j++;
  return;
}
