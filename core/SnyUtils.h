#ifndef _STREAMINGNOLOGY_CORE_SNYUTILS_H_
#define _STREAMINGNOLOGY_CORE_SNYUTILS_H_
#include "SnyType.h"
#include <string>
#include <vector>
#include "SnyDataBuffer.h"
#include "SnyConstants.h"
#include "http_server.h"
#include "http_headers.h"
#include "http_content_type.h"

namespace SNYUtils {
  SNY_UI64 convertTime(SNY_UI64 time_value, SNY_UI64 from_time_scale, SNY_UI64 to_time_scale);
  std::string  formatstring(const char *format, ...);
  std::vector<std::string> splitUri(std::string uri);
  SnyDataBuffer listDirectoryContent(std::string path);
  std::vector<std::string> getIpAddress();
  std::string getCurrentAppDirectory();
  SNYSERVICETYPE getServiceTypeByPath(const std::string path);
  void makeNotFoundResponse(Network::IHttpRequestPtr req);
}

#endif /*_STREAMINGNOLOGY_CORE_SNYUTILS_H_*/
