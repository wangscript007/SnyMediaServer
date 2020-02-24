#include "SnyUtils.h"
#include "SnyType.h"
#include <regex>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include "SnyPlatform.h"
#include <algorithm>
#include <cctype>
#if defined(Q_OS_WIN32)
#include <fstream>
#include <iostream>
extern "C" {
#include <stdio.h>
#include <string.h> /* for strncpy */
#include <sys/types.h>
#include <event2/http.h>
}
#include <Windows.h>
#include <experimental/filesystem>
#include <filesystem>
using namespace std;
namespace fs = std::experimental::filesystem;
#else
#include <dirent.h>
extern "C" {
#include <stdio.h>
#include <unistd.h>
#include <string.h> /* for strncpy */
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>

#include <event2/http.h>
}
#endif // Q_OS_WIN32
using namespace Network;
using namespace std;
namespace SNYUtils {
  SNY_UI64 convertTime(SNY_UI64 time_value, SNY_UI64 from_time_scale, SNY_UI64 to_time_scale) {
    if (from_time_scale == 0) return 0;
    double ratio = (double)to_time_scale/(double)from_time_scale;
    return ((SNY_UI64)(0.5+(double)time_value*ratio));
  }
  
  string formatstring(const char *format, ...) {
  const int BUFFER_SIZE = 9216;
  char strbuf[BUFFER_SIZE];
  va_list ap;
  va_start (ap, format);
#ifdef _WIN32
  int result = vsnprintf_s(strbuf, BUFFER_SIZE, format, ap);
#else
  int result = vsnprintf(strbuf, BUFFER_SIZE, format, ap);
#endif
  va_end (ap);
  return string(strbuf, result);
  }
  
  std::vector<std::string> splitUri(std::string uri) {
    std::vector<std::string> vec;
    
    std::regex words_regex("([^/]+)");
    auto words_begin = std::sregex_iterator(uri.begin(), uri.end(), words_regex);
    auto words_end   = std::sregex_iterator();
    
    for (auto i = words_begin; i != words_end; ++i) {
      std::smatch match = *i;
      std::string match_str = match.str();
      vec.push_back(match_str);
    }

    return vec;
  }

  SnyDataBuffer listDirectoryContent(std::string path) {
    SnyDataBuffer content;
    
    std::vector<string> fileList;
    std::vector<string> folderList;
#if defined(Q_OS_WIN32)
    if (path.empty()) {
      DWORD dwSize = MAX_PATH;
      char szLogicalDrives[MAX_PATH] = {0};
      DWORD dwResult = GetLogicalDriveStrings(dwSize,szLogicalDrives);
      if (dwResult > 0 && dwResult <= MAX_PATH) {
        char* szSingleDrive = szLogicalDrives;
        while(*szSingleDrive) {
          folderList.push_back(szSingleDrive);
          szSingleDrive += strlen(szSingleDrive) + 1;
        }
      }
    } else {
      fs::path u8_path = fs::u8path(path);
      for ( auto & fe : fs::directory_iterator(u8_path) ) {
        auto fp = fe.path();
        auto fFiename = fp.filename();
        if ( fs::is_directory( fe ) && !fs::is_empty( fe ) ) {
          if ( fFiename != "." && fFiename != ".." ) {
            folderList.push_back(fFiename.generic_u8string());
          }
        } else if (fs::is_regular_file( fe )) {
          std::string ext;
          if (fp.has_extension()) {
            ext = fp.extension().string();
            std::transform( ext.begin(), ext.end(), ext.begin(),
                   [](unsigned char c){ return std::tolower(c); } );
            if (ext.compare(".mp4") == 0) {
              fileList.push_back(fFiename.generic_u8string());
            }
          }
        }
      }
    }
#elif defined(Q_OS_MACX)
    if (path.empty()) {
      folderList.push_back("/Users");
      folderList.push_back("/Volumes");
    } else {
      DIR *dp;
      struct dirent *dirp;
      if ((dp = opendir(path.c_str())) == NULL) {
        return content;
      }
      while ((dirp = readdir(dp)) != NULL) {
        if (dirp->d_type == DT_REG) {
          string name(dirp->d_name, dirp->d_namlen);
          if (name.compare(0, 1, ".") != 0) {
            string ext = name.substr(name.find_last_of('.') + 1);
            std::transform( ext.begin(), ext.end(), ext.begin(),
                   [](unsigned char c){ return std::tolower(c); } );
            if (ext.compare("mp4") == 0) {
              fileList.push_back(name);
            }
          }
        }
        else if (dirp->d_type == DT_DIR) {
          string name(dirp->d_name, dirp->d_namlen);
          if (name.compare(".") != 0 && name.compare("..") != 0 && name.compare(0, 1, ".") != 0) {
            folderList.push_back(name);
          }
        }
      }
      closedir(dp);
  }
#else
    if (path.empty()) {
      folderList.push_back("/Users");
      folderList.push_back("/Volumes");
    } else {
      DIR *dp;
      struct dirent *dirp;
      if ((dp = opendir(path.c_str())) == NULL) {
        return content;
      }
      while ((dirp = readdir(dp)) != NULL) {
        if (dirp->d_type == DT_REG) {
          string name(dirp->d_name, dirp->d_namlen);
          if (name.compare(0, 1, ".") != 0) {
            fileList.push_back(name);
          }
        }
        else if (dirp->d_type == DT_DIR) {
          string name(dirp->d_name, dirp->d_namlen);
          if (name.compare(".") != 0 && name.compare("..") != 0 && name.compare(0, 1, ".") != 0) {
            folderList.push_back(name);
          }
        }
      }
      closedir(dp);
    }
#endif // Q_OS_WIN32

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    writer.StartObject();
    writer.Key("status");
    writer.Int(200);
    
    writer.Key("data");
    writer.StartArray();
    for (auto iter = folderList.begin(); iter != folderList.end(); iter++) {
      std::string fileName = *iter;
      std::string fullName = *iter;
      if (path.empty()) {
        //TODO:
      } else {
#if defined(Q_OS_WIN32)
        std::filesystem::path p = path;
        std::filesystem::path fp = p / *iter;
        fullName = fp.string();
#else
        fullName = path + "/" + *iter;
#endif
      }
      
      writer.StartObject();
      writer.Key("label");
      writer.String(fileName.data(), static_cast<rapidjson::SizeType>(fileName.size()));
      writer.Key("path");
      writer.String(fullName.data(), static_cast<rapidjson::SizeType>(fullName.size()));
      writer.Key("last");
      writer.Bool(false);
      writer.EndObject();
    }
    for (auto iter = fileList.begin(); iter != fileList.end(); iter++) {
      std::string fileName = *iter;
      std::string fullName = path + "/" + *iter;
      writer.StartObject();
      writer.Key("label");
      writer.String(fileName.data(), static_cast<rapidjson::SizeType>(fileName.size()) );
      writer.Key("path");
      writer.String(fullName.data(), static_cast<rapidjson::SizeType>(fullName.size()));
      writer.Key("last");
      writer.Bool(true);
      writer.EndObject();
    }
    writer.EndArray();
    
    writer.EndObject();
    
    std::string s = buffer.GetString();
    content.append(s.data(), static_cast<int>(s.size()));
    
    return content;
  }
  
  std::vector<std::string> getIpAddress() {
    std::vector<std::string> ips;
#if defined(Q_OS_WIN32)
#else
    const char* network_name[] = { "eth0","wlan0","en0" };
    for (int i = 0; i < 3; i++) {
      int fd; struct ifreq ifr;
      fd = socket(AF_INET, SOCK_DGRAM, 0);
      /* I want to get an IPv4 IP address */
      ifr.ifr_addr.sa_family = AF_INET;
      /* I want IP address attached to "eth0" */
      strncpy(ifr.ifr_name, network_name[i], IFNAMSIZ - 1);
      int err = ioctl(fd, SIOCGIFADDR, &ifr);
      close(fd);
      if (err != -1) {
        string ip(inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
        ips.push_back(ip);
      }
    }
#endif // Q_OS_WIN32
    return ips;
  }
  
  std::string getCurrentAppDirectory(){
    std::string pwd;
    char* path;
    path = getenv ("PWD");
    if (path!=NULL) {
      pwd = path;
    }
    return pwd;
  }
  
  SNYSERVICETYPE getServiceTypeByPath(const std::string path) {
    SNYSERVICETYPE snyServiceType = SNY_SERVICE_UNKNOWN;
    if (path.compare(SNY_MEDIASERVER_SYSTEM_C) == 0) {
      snyServiceType = SNY_SERVICE_SYSTEM;
    } else if (path.compare(SNY_MEDIASERVER_MEDIA_C) == 0) {
      snyServiceType = SNY_SERVICE_MEDIA;
    } else if (path.compare(SNY_MEDIASERVER_WEB_C) == 0) {
      snyServiceType = SNY_SERVICE_WEB;
    } else if (path.compare(SNY_MEDIASERVER_VOD_C) == 0) {
      snyServiceType = SNY_SERVICE_MEDIA_VOD;
    } else if (path.compare(SNY_MEDIASERVER_LIVE_C) == 0) {
      snyServiceType = SNY_SERVICE_MEDIA_LIVE;
    } else if (path.compare(SNY_MEDIASERVER_MEDIA_GETMEDIAINFO_C) == 0) {
      snyServiceType = SNY_SERVICE_MEDIA_GETMEDIAINFO;
    } else if (path.compare(SNY_MEDIASERVER_SYSTEM_GETDIRECTORYCONTENT_C) == 0) {
      snyServiceType = SNY_SERVICE_SYSTEM_GETDIRECTORYCONTENT;
    } else if (path.compare(SNY_MEDIASERVER_SYSTEM_GETSYSTEMINFO_C) == 0) {
      snyServiceType = SNY_SERVICE_SYSTEM_GETSYSTEMINFO;
    } else {
      snyServiceType = SNY_SERVICE_UNKNOWN;
    }
    return snyServiceType;
  }
  
  void makeNotFoundResponse(IHttpRequestPtr req) {
    req->SetResponseCode(HTTP_NOTFOUND);
  }
  
}

