#include "rapidjson/document.h"
#include "http_server.h"
#include "http_headers.h"
#include "http_content_type.h"
#include "SnyUtils.h"
#include "SnyMediaService.h"
using namespace Network;

SNYMediaService::SNYMediaService() { 
  this->VODManager = new SNYVODManager();
  VODManager->start();
  this->LIVEManager = new SNYLiveManager();
}

SNYMediaService::~SNYMediaService() {
  if (VODManager != nullptr) {
    delete VODManager; VODManager = nullptr;
  }
}

void SNYMediaService::requestContent(Network::IHttpRequestPtr req) {
  std::string path = req->GetPath();

  std::vector<std::string> paths = SNYUtils::splitUri(path);
  if (paths.size() < 2) {
    SNYUtils::makeNotFoundResponse(req);
    return;
  }

  SNYSERVICETYPE secondLevelType = SNYUtils::getServiceTypeByPath(paths[1]);
  if (secondLevelType == SNY_SERVICE_MEDIA_VOD) {
    processVod(req);
  } else if (secondLevelType == SNY_SERVICE_MEDIA_LIVE) {
    processLive(req);
  } else if (secondLevelType == SNY_SERVICE_MEDIA_GETMEDIAINFO) {
    getMediaInfo(req);
  } else {
    SNYUtils::makeNotFoundResponse(req);
  }
}

void SNYMediaService::getMediaInfo(IHttpRequestPtr req) {
  const IHttpRequest::Type requestType = req->GetRequestType();
  if (requestType != IHttpRequest::Type::POST) {
    SNYUtils::makeNotFoundResponse(req);
    return;
  }
  SnyDataBuffer content;
  const std::size_t content_size = req->GetContentSize();
  if (content_size <= SNY_MEDIASERVER_ACCEPT_MAX_CONTENT_SIZE) {
    content.resize(static_cast<int>(content_size));
    req->GetContent(content.data(), content_size, false);
  }
  rapidjson::Document document;
  document.Parse(content.data());
  if (!document.IsObject()) {
    SNYUtils::makeNotFoundResponse(req);
    return;
  }
  if (!document.HasMember(SNY_MEDIASERVER_MEDIA_GETMEDIAINFO_PATH_C.data())) {
    SNYUtils::makeNotFoundResponse(req);
    return;
  }
  
  std::string file_path = document[SNY_MEDIASERVER_MEDIA_GETMEDIAINFO_PATH_C.data()].GetString();
  std::string host = req->GetHeaderAttr(Http::Request::Header::Host::Value);
  if (VODManager != nullptr) {
    SnyDataBuffer db = VODManager->createChannel(host, file_path);
    req->SetResponseBuf(db.data(), db.size());
  }
}

void SNYMediaService::processLive(IHttpRequestPtr req) {
  if (LIVEManager != nullptr) {
    LIVEManager->requestContent(req);
  }
  return;
}

void SNYMediaService::processVod(IHttpRequestPtr req) {
  if (VODManager != nullptr) {
    VODManager->requestContent(req);
  }
}



