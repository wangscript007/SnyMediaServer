#ifndef _STREAMINGNOLOGY_MEDIASERVER_SNYBROADCASTSERVICE_H_
#define _STREAMINGNOLOGY_MEDIASERVER_SNYBROADCASTSERVICE_H_
#include "SnyIEventCallback.h"
#include "SnyThreads.h"
#include "SnyType.h"

class SnyBroadcastService {
public:
  SnyBroadcastService(SNY_UI16 port);
  ~SnyBroadcastService();
public:
  void start();
  void stop();
  int onThreadProc(int id);
  void setMediaServerPort(SNY_UI16 srvPort) { this->servicePort = srvPort; }
  SNY_UI16 getMediaServerPort() { return this->servicePort; }
  struct event_base* getBase() { return base; }
public:
  static void onEventCallback(const int sock, short int which, void *arg);
private:
  void terminateLoop();
private:
  SNY_UI16       broadcastPort;
  SNY_UI16       servicePort;
  int            srvSocket;
  struct event_base            *base;
  Threads<SnyBroadcastService> threads;
};

#endif
