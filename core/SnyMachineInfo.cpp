#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include "SnyPlatform.h"
#include "SnyMachineInfo.h"

#if defined(Q_OS_WIN32)
#include "utsname.h"
extern "C" {
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
}
#else
extern "C" {
#include <sys/utsname.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
}
#endif // Q_OS_WIN32

SNYMachineInfo::SNYMachineInfo(SNY_UI16 lp) {
  this->listeningPort = lp;
}

void SNYUTSName::operator=(const SNYUTSName &object) { 
  this->sysname  = object.sysname;
  this->nodename = object.nodename;
  this->release  = object.release;
  this->version  = object.version;
  this->machine  = object.machine;
}

SNYUTSName::SNYUTSName(const SNYUTSName &object) { 
  operator=(object);
}

SNYUTSName SNYMachineInfo::getUTSName() {
  SNYUTSName uts_name;
  struct utsname buf;
  if(uname(&buf)) {
    return uts_name;
  }
  
  uts_name.sysname  = buf.sysname;
  uts_name.nodename = buf.nodename;
  uts_name.release  = buf.release;
  uts_name.version  = buf.version;
  uts_name.machine  = buf.machine;
  
  return uts_name;
}

std::vector<std::string> SNYMachineInfo::getIpAddress() {
  std::vector<std::string> ips;
#if defined(Q_OS_WIN32)
#else
  const char* network_name[] = {"eth0","wlan0","en0"};
  for (int i = 0; i < 3; i++) {
    int fd; struct ifreq ifr;
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    /* I want to get an IPv4 IP address */
    ifr.ifr_addr.sa_family = AF_INET;
    /* I want IP address attached to "eth0" */
    strncpy(ifr.ifr_name, network_name[i], IFNAMSIZ-1);
    int err = ioctl(fd, SIOCGIFADDR, &ifr);
    close(fd);
    if (err != -1) {
      std::string ip(inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
      ips.push_back(ip);
    }
  }
#endif // Q_OS_WIN32
  return ips;
}

SnyDataBuffer SNYMachineInfo::getMachineInfoinJSON() {
  std::vector<std::string> ips = getIpAddress();
  SNYUTSName utsName = getUTSName();
  
  SnyDataBuffer playlist_buffer;
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  writer.StartObject();
  
  writer.Key("appVersion");
  writer.String(appVersion.c_str());
  
  writer.Key("uts");
  writer.StartArray();
  writer.StartObject();
  writer.Key("sysname");
  writer.String(utsName.sysname.c_str());
  writer.Key("nodename");
  writer.String(utsName.nodename.c_str());
  writer.Key("release");
  writer.String(utsName.release.c_str());
  writer.Key("version");
  writer.String(utsName.version.c_str());
  writer.Key("machine");
  writer.String(utsName.machine.c_str());
  writer.EndObject();
  writer.EndArray();
  
  writer.Key("listeningPort");
  writer.Uint(listeningPort);
  
  writer.Key("ip");
  writer.StartArray();
  
  int i = 0;
  for (auto iter = ips.begin(); iter != ips.end(); iter++, i++) {
    writer.StartObject();
    writer.Key(std::to_string(i).data());
    writer.String(iter->data());
    writer.EndObject();
  }
  if (ips.empty()) {
    writer.StartObject();
    writer.Key(std::to_string(i).data());
    writer.String("localhost");
    writer.EndObject();
  }
  writer.EndArray();
  
  writer.EndObject();
  
  std::string s = buffer.GetString();
  playlist_buffer.append(s.data(), static_cast<int>(s.size()));

  return playlist_buffer;
}

void SNYMachineInfo::setAppVersion(std::string ver) { 
    this->appVersion = ver;
}





