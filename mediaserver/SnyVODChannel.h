#ifndef _STREAMINGNOLOGY_MEDIASERVER_SNYVODCHANNEL_H_
#define _STREAMINGNOLOGY_MEDIASERVER_SNYVODCHANNEL_H_
#include <mutex>
#include <vector>
#include "SnyType.h"
#include "SnyIDemuxer.h"
#include "http_request.h"
#include "SnyVODMediaManifest.h"
#include "SnyPlaylist.h"

class SNYVODChannel {
public:
  SNYVODChannel(std::string channelID);
  ~SNYVODChannel();
public:
  SNY_Bool getPlaybackUri(std::string host, SNYPlaylists& playlist);
  std::string   getChannelID();
  void     setUri(std::string uri);
  std::string   getUri() { return uri; }
  void     requestContent(Network::IHttpRequestPtr req);
  void     release();
private:
  SNY_Bool internalMediaOpen();
  SNY_Bool internalMediaSeekTo(SNY_SI64 position);
  SNY_Bool internalMediaReadTo(SNY_SI64 position);
  SNY_Bool internalMuxSegment(SNY_SI64 position, Network::IHttpRequestPtr req);
  void     internalSegmentNotFound(Network::IHttpRequestPtr req);
  void     internalProcessMediaManifestRequest(Network::IHttpRequestPtr req);
  void     internalProcessSegmentRequest(Network::IHttpRequestPtr req);
  void     internalSaveDataToRequest(Network::IHttpRequestPtr req, const SNY_UI08* data, const SNY_SI32 size);
  void     internalMediaManifest();
private:
  std::string channelID;
  std::string uri;
  std::vector<SNY_UI32> selectedTrackNumbers;
  SNYVODMediaManifest* manifest;
  GPACMediaInfo mediaInfo;
  SNYIDemuxer* demuxer;
  SNY_SI64 currentSegmentIndex;
  SNY_Bool inited;
  std::list<SnySample*> samples;
};

#endif //!_STREAMINGNOLOGY_MEDIASERVER_SNYVODCHANNEL_H_
