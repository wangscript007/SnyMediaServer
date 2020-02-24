#ifndef _STREAMINGNOLOGY_MEDIASERVER_SNYLIVEMANAGER_H_
#define _STREAMINGNOLOGY_MEDIASERVER_SNYLIVEMANAGER_H_

#include "SnyType.h"
#include "SnyDataBuffer.h"
#include "http_request.h"

class SNYLiveManager {
public:
  SNYLiveManager();
  ~SNYLiveManager();
public:
  void requestContent(Network::IHttpRequestPtr req);
  SNY_Bool createChannel(const std::string channel_uuid);
  SNY_Bool destroyChannel(const std::string channel_uuid);
  SNY_Bool startChannel(const std::string channel_uuid);
  SNY_Bool stopChannel(const std::string channel_uuid);
  SnyDataBuffer getAllChannelInfo();
  SnyDataBuffer getChannelInfo(const std::string channel_uuid);
  void updateLiveChannel(const std::string channel_uuid, const char* config_data, const SNY_SI32 config_length);
};

#endif //!_STREAMINGNOLOGY_MEDIASERVER_SNYLIVEMANAGER_H_
