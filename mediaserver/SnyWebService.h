#ifndef _STREAMINGNOLOGY_MEDIASERVER_SNYWEBSERVICE_H_
#define _STREAMINGNOLOGY_MEDIASERVER_SNYWEBSERVICE_H_
#include "http_request.h"

class SNYWebService {
public:
  SNYWebService(std::string web);
  ~SNYWebService();
  void requestContent(Network::IHttpRequestPtr req);
private:
  std::string webroot;
};

#endif //!_STREAMINGNOLOGY_MEDIASERVER_SNYWEBSERVICE_H_
