#include <iostream>
#include <string>
#include <vector>
#include "http_server.h"
#include "http_headers.h"
#include "http_content_type.h"
#include "easylogging++.h"
#include "SnyUtils.h"
#include "SnyMediaServer.h"
#include "jwt-cpp/jwt.h"
using namespace Network;
using namespace std;

INITIALIZE_EASYLOGGINGPP
SNYMediaServer::SNYMediaServer(){
  this->srvThreadCount   = 4;
  this->srvPort          = 8064;
  this->broadcastPort    = 8065;
  this->httpServer       = nullptr;
  this->mediaService     = nullptr;
  this->systemService    = nullptr;
  this->webService       = nullptr;
  this->broadcastService = nullptr;
}

SNYMediaServer::~SNYMediaServer() {
  if (mediaService != nullptr) {
    delete mediaService; mediaService = nullptr;
  }
  if (systemService != nullptr) {
    delete systemService; systemService = nullptr;
  }
}

void SNYMediaServer::setServiceThreadCount(SNY_UI16 stc) { 
  this->srvThreadCount = stc;
}

void SNYMediaServer::setListeningPort(SNY_UI16 lp) { 
  this->srvPort = lp;
}

void SNYMediaServer::setAdminWebDirectory(std::string webroot) {
  this->adminWebroot = webroot;
}

void SNYMediaServer::start() {
    // Load configuration from file
  el::Configurations conf("/Users/developer/Documents/sny/streamingnology/log/el.conf");
    // Reconfigure single logger
  el::Loggers::reconfigureLogger("default", conf);
    // Actually reconfigure all loggers instead
  el::Loggers::reconfigureAllLoggers(conf);
    // Now all the loggers will use configuration from file
  
  try {
    char const srvAddress[] = "0.0.0.0";
    httpServer = new HttpServer(srvAddress, srvPort, srvThreadCount, onRequestFunction, this);
  }
  catch (std::exception const &e) {
    std::cout << e.what() << std::endl;
  }
  if (mediaService == nullptr) {
    mediaService = new SNYMediaService();
  }
  if (systemService == nullptr) {
    systemService = new SNYSystemService();
    systemService->setListeningPort(srvPort);
  }
  if (webService == nullptr) {
    webService = new SNYWebService(this->adminWebroot);
  }
  if (broadcastService == nullptr) {
    broadcastService = new SnyBroadcastService(broadcastPort);
    broadcastService->setMediaServerPort(srvPort);
    broadcastService->start();
  }
}

void SNYMediaServer::stop() {
  return;
}

void SNYMediaServer::restart() {
  return;
}

void SNYMediaServer::onRequest(Network::IHttpRequestPtr req) {
  std::lock_guard<std::recursive_mutex> lock(mutex);
  requestNumInProcessing++;
  req->SetResponseAttr(Http::Response::Header::Server::Value, COMPANYNAME);
  std::string origin = req->GetHeaderAttr("Origin");
  if (!origin.empty()) {
    req->SetResponseAttr(Http::Response::Header::AccessControlAllowOrigin::Value, origin);
  }
  std::string reqHeaders = req->GetHeaderAttr("Access-Control-Request-Headers");
  if (!reqHeaders.empty()) {
    req->SetResponseAttr(Http::Response::Header::AccessControlAllowHeaders::Value, reqHeaders);
  }
  
  const IHttpRequest::Type requestType = req->GetRequestType();
  if (requestType == IHttpRequest::Type::OPTION) {
    req->SetResponseAttr(Http::Response::Header::AccessControlAllowMethods::Value, "POST, GET, OPTIONS");
    //req->SetResponseAttr(Http::Response::Header::AccessControlAllowCredentials::Value, "true");
    LOG(DEBUG)<<req->RequestToString();
    return;
  }
  
  const std::string path = req->GetPath();
  req->SetResponseAttr(Http::Response::Header::ContentType::Value, Http::Content::TypeFromFileName(path));
  std::vector<std::string> paths = SNYUtils::splitUri(path);
  if (paths.empty()) {
    SNYUtils::makeNotFoundResponse(req);
    LOG(DEBUG)<<req->RequestToString();
    return;
  }
  
  SNYSERVICETYPE firstLevelServiceType = SNYUtils::getServiceTypeByPath(paths[0]);
  
  if (firstLevelServiceType == SNY_SERVICE_SYSTEM) {
    if (systemService != nullptr) {
      systemService->requestContent(req);
    }
  } else if (firstLevelServiceType == SNY_SERVICE_MEDIA) {
    if (mediaService != nullptr) {
      mediaService->requestContent(req);
    }
  } else if (firstLevelServiceType == SNY_SERVICE_WEB) {
    if (webService != nullptr) {
      webService->requestContent(req);
    }
  } else {
    if (webService != nullptr) {
      webService->requestContent(req);
      LOG(DEBUG)<<req->RequestToString();
    }
    //SNYUtils::makeNotFoundResponse(req);
  }
  LOG(DEBUG)<<req->RequestToString();
}

void SNYMediaServer::onRequestFunction(Network::IHttpRequestPtr req, void* user) {
  SNYMediaServer* snyMediaServer = dynamic_cast<SNYMediaServer*>((SNYMediaServer*)user);
  snyMediaServer->onRequest(req);
}

