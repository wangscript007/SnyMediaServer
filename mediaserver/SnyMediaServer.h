#ifndef _STREAMINGNOLOGY_MEDIASERVER_SNY_MEDIASERVER_H_
#define _STREAMINGNOLOGY_MEDIASERVER_SNY_MEDIASERVER_H_
#include "http_request.h"
#include "http_server.h"
#include "SnyType.h"
#include "SnyThreads.h"
#include "SnyMediaService.h"
#include "SnySystemService.h"
#include "SnyWebService.h"
#include "SnyBroadcastService.h"

using namespace Network;
class SNYMediaServer {
public:
  SNYMediaServer();
  ~SNYMediaServer();
public:
  void setListeningPort(SNY_UI16 lp);
  void setServiceThreadCount(SNY_UI16 stc);
  void setAdminWebDirectory(std::string webroot);
  void start();
  void stop();
  void restart();
public:
  void onRequest(Network::IHttpRequestPtr req);
private:
  static void onRequestFunction(Network::IHttpRequestPtr req, void* user);
public:
  std::recursive_mutex mutex;
  std::atomic<int>     requestNumInProcessing;
  std::atomic<SNY_Bool> refuseRequest;
  HttpServer*          httpServer;
  std::string          adminWebroot;
  SNY_UI16             srvPort;
  SNY_SI16             broadcastPort;
  SNY_UI16             srvThreadCount;
  SNYMediaService*     mediaService;
  SNYSystemService*    systemService;
  SNYWebService*       webService;
  SnyBroadcastService* broadcastService;
};

#endif //!_STREAMINGNOLOGY_MEDIASERVER_SNY_MEDIASERVER_H_
