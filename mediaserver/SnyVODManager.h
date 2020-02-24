#ifndef _STREAMINGNOLOGY_MEDIASERVER_SNYVODMANAGER_H_
#define _STREAMINGNOLOGY_MEDIASERVER_SNYVODMANAGER_H_
#include <string>
#include <chrono>
#include <map>
#include "SnyType.h"
#include "SnyThreads.h"
#include "http_request.h"
#include "SnyVODChannel.h"
#include "SnyPlaylist.h"

#define MAX_UNUSED_CHANNEL_DURATION   60
#define MAX_CHANNEL_CNT_ACTIVE        15
#define MAX_CHANNEL_CNT_INACTIVE      100

class SNYVODManager {
public:
  SNYVODManager();
  ~SNYVODManager();
public:
  void requestContent(Network::IHttpRequestPtr req);
  SnyDataBuffer createChannel(std::string host, std::string file_path);
  void start();
  void stop();
  int onThreadProc(int id);
private:
  SNY_Bool internalCreateChannel(std::string md5_value, std::string file_path);
  SNY_Bool isChannelExist(std::string file_path_md5_value, SNYVODChannel*& channel);
  SNY_Bool isChannelAtInActive(std::string file_path_md5_value, std::string &file_path);
  void     releaseResInternal();
  void     deleteChannelInactive(std::string file_path_md5_value);
private:
  std::recursive_mutex                                            mutex;
  Threads<SNYVODManager>                                          threads;
  std::map<std::string, SNYVODChannel*>                           SnyChannels;
  std::map<std::string, std::string>                              SnyChannelsInactive;
  std::map<std::string, std::chrono::system_clock::time_point>    SnyChannelsInactiveTick;
  std::map<std::string, std::chrono::system_clock::time_point>    SnyChannelRequestTick;
};

#endif //!_STREAMINGNOLOGY_MEDIASERVER_SNYVODMANAGER_H_
