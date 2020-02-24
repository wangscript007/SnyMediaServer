#include "SnyWebService.h"
#include "http_content_type.h"
#include "http_server.h"
#include "http_headers.h"
#include <unistd.h>
#include "easylogging++.h"
using namespace std;
using namespace Network;
SNYWebService::SNYWebService(std::string web) {
  const int MAXPATH=250;
  char buffer[MAXPATH];
  getcwd(buffer, MAXPATH);
  //printf("The current directory is: %s \n", buffer);
  std::string pt(buffer);
  LOG(DEBUG)<<pt;
  if (!pt.empty() && pt.back() != '/') {
    pt.push_back('/');
  }
  pt += "./SnyMediaServerApp.app/Contents/Resources/web";
  LOG(DEBUG)<<pt;
  LOG(DEBUG)<<web;
  this->webroot = web;
}

SNYWebService::~SNYWebService() {
  ;
}

void SNYWebService::requestContent(Network::IHttpRequestPtr req) {
  string file = webroot + req->GetPath();
  req->SetResponseAttr(Http::Response::Header::ContentType::Value, Http::Content::TypeFromFileName(file));
  req->SetResponseFile(file);
}


