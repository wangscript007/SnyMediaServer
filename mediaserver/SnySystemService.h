#ifndef _STREAMINGNOLOGY_MEDIASERVER_SNY_SYSTEMSERVICE_H_
#define _STREAMINGNOLOGY_MEDIASERVER_SNY_SYSTEMSERVICE_H_
#include "http_request.h"
#include "SnyType.h"

class SNYSystemService {
public:
  SNYSystemService();
  ~SNYSystemService();
  void setListeningPort(SNY_UI16 lp);
  void requestContent(Network::IHttpRequestPtr req);
private:
  SNY_UI16 listeningPort;
};

#endif//!_STREAMINGNOLOGY_MEDIASERVER_SNY_SYSTEMSERVICE_H_
