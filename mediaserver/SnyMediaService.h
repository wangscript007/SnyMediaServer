#ifndef _STREAMINGNOLOGY_MEDIASERVER_SNYMEDIASERVICE_H_
#define _STREAMINGNOLOGY_MEDIASERVER_SNYMEDIASERVICE_H_
#include <mutex>
#include "http_request.h"
#include "SnyConstants.h"
#include "SnyVODManager.h"
#include "SnyLiveManager.h"

class SNYMediaService {
public:
	SNYMediaService();
	~SNYMediaService();
  void requestContent(Network::IHttpRequestPtr req);
private:
  void processVod(Network::IHttpRequestPtr req);
  void processLive(Network::IHttpRequestPtr req);
  void getMediaInfo(Network::IHttpRequestPtr req);
private:
  SNYVODManager* VODManager;
  SNYLiveManager* LIVEManager;
};

#endif //!_STREAMINGNOLOGY_MEDIASERVER_SNYMEDIASERVICE_H_
