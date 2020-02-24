#ifndef _STREAMINGNOLOGY_CORE_SNY_MACHINE_INFO_H_
#define _STREAMINGNOLOGY_CORE_SNY_MACHINE_INFO_H_
#include<string>
#include<vector>
#include "SnyDataBuffer.h"
#include "SnyType.h"

class SNYUTSName {
public:
  SNYUTSName() {}
  ~SNYUTSName() {}
  SNYUTSName(const SNYUTSName &object);
  void operator = (const SNYUTSName &object);
public:
  std::string sysname;
  std::string nodename;
  std::string release;
  std::string version;
  std::string machine;
};

class SNYMachineInfo {
public:
  SNYMachineInfo(SNY_UI16 lp);
  ~SNYMachineInfo() {}
  void setAppVersion(std::string ver);//AppName-1.0.1-Aug.19.2018-git
public:
  std::string   appVersion;
  SNY_UI16 listeningPort;
  std::vector<std::string> getIpAddress();
  SNYUTSName getUTSName();
  SnyDataBuffer getMachineInfoinJSON();
public:
  
};

#endif
