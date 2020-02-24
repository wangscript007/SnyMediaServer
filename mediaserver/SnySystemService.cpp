#include <string>
#include <vector>
#include "rapidjson/document.h"
#include "http_content_type.h"
#include "http_server.h"
#include "http_headers.h"
#include "SnyMachineInfo.h"
#include "SnyUtils.h"
#include "SnyVersion.h"
#include "SnySystemService.h"

using namespace std;
using namespace Network;
SNYSystemService::SNYSystemService() {
  this->listeningPort = 8064;
}

SNYSystemService::~SNYSystemService() {
}

void SNYSystemService::setListeningPort(SNY_UI16 lp) {
  this->listeningPort = lp;
}

void SNYSystemService::requestContent(Network::IHttpRequestPtr req) {
  const IHttpRequest::Type requestType = req->GetRequestType();
  const std::string path = req->GetPath();
  std::vector<std::string> paths = SNYUtils::splitUri(path);
  if (paths.size() < 2) {
    SNYUtils::makeNotFoundResponse(req);
    return;
  }
  SNYSERVICETYPE serviceType = SNYUtils::getServiceTypeByPath(paths[1]);
  if (serviceType == SNY_SERVICE_SYSTEM_GETDIRECTORYCONTENT) {
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
    SnyDataBuffer dc = SNYUtils::listDirectoryContent(file_path);
    req->SetResponseBuf(dc.data(), dc.size());
    req->SetResponseAttr(Http::Response::Header::ContentType::Value, "application/json;charset=UTF-8");
  } else if (serviceType == SNY_SERVICE_SYSTEM_GETSYSTEMINFO) {
    SNYMachineInfo machineInfo(listeningPort);
    machineInfo.setAppVersion(MEDIASERVER_FULL_VERSION);
    SnyDataBuffer buffer = machineInfo.getMachineInfoinJSON();
    req->SetResponseBuf(buffer.data(), buffer.size());
    req->SetResponseAttr(Http::Response::Header::ContentType::Value, "application/json;charset=UTF-8");
  } else {
    SNYUtils::makeNotFoundResponse(req);
  }
  return;
}
