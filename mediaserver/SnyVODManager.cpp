#include <vector>
#include "md5.h"
#include "SnyUtils.h"
#include "SnyVODManager.h"

using namespace Network;
SNYVODManager::SNYVODManager():threads(this) {
  ;
}

SNYVODManager::~SNYVODManager() {
  ;
}

void SNYVODManager::requestContent(IHttpRequestPtr req) {
  std::lock_guard<std::recursive_mutex> lock(mutex);
  std::vector<std::string> dir = SNYUtils::splitUri(req->GetPath());
  if (dir.size() < 3) {
    SNYUtils::makeNotFoundResponse(req);
    return;
  }
  std::string channelID = dir[2];
  SNYVODChannel *channel = nullptr; std::string file_path;
  if (isChannelExist(channelID, channel)) {
    if (channel != nullptr) {
      auto now = std::chrono::system_clock::now();
      SnyChannelRequestTick[channelID] = now;
      channel->requestContent(req);
    } else {
      SNYUtils::makeNotFoundResponse(req);
    }
  } else if(isChannelAtInActive(channelID, file_path)) {
    internalCreateChannel(channelID, file_path);
    if (isChannelExist(channelID, channel)) {
      if (channel != nullptr) {
        channel->requestContent(req);
      }
    } else {
      SNYUtils::makeNotFoundResponse(req);
    }
    deleteChannelInactive(channelID);
  } else {
    SNYUtils::makeNotFoundResponse(req);
  }
}

SnyDataBuffer SNYVODManager::createChannel(std::string host, std::string file_path) {
  std::lock_guard<std::recursive_mutex> lock(mutex);
  SnyDataBuffer databuffer;
  std::string md5_value = MD5(file_path).hexdigest();
  SNYVODChannel* channel = nullptr;
  
  if (!isChannelExist(md5_value, channel)) {
    internalCreateChannel(md5_value, file_path);
  }
  
  if (isChannelExist(md5_value, channel)) {
    if (channel != nullptr) {
      SNYPlaylists playlists;
      if (channel->getPlaybackUri(host, playlists)) {
        databuffer = playlists.toJSON();
      }
    } else {
        //TODO:
    }
  }
  return databuffer;
}

bool SNYVODManager::internalCreateChannel(std::string md5_value, std::string file_path) {
  std::lock_guard<std::recursive_mutex> lock(mutex);
  SNYVODChannel* channel = new SNYVODChannel(md5_value);
  channel->setUri(file_path);
  SnyChannels.insert(make_pair(md5_value, channel));
  auto now = std::chrono::system_clock::now();
  SnyChannelRequestTick.insert(make_pair(md5_value, now));
  return true;
}

SNY_Bool SNYVODManager::isChannelExist(std::string file_path_md5_value, SNYVODChannel*& channel) {
  std::lock_guard<std::recursive_mutex> lock(mutex);
  auto iter = SnyChannels.find(file_path_md5_value);
  if (iter != SnyChannels.end()) {
    channel = iter->second;
    return true;
  }
  return false;
}

SNY_Bool SNYVODManager::isChannelAtInActive(std::string file_path_md5_value, std::string &file_path) {
  std::lock_guard<std::recursive_mutex> lock(mutex);
  auto iter = SnyChannelsInactive.find(file_path_md5_value);
  if (iter != SnyChannelsInactive.end()) {
    file_path = iter->second;
    return true;
  }
  return false;
}

void SNYVODManager::deleteChannelInactive(std::string file_path_md5_value) {
  std::lock_guard<std::recursive_mutex> lock(mutex);
  auto iter = SnyChannelsInactive.find(file_path_md5_value);
  if (iter != SnyChannelsInactive.end()) {
    SnyChannelsInactive.erase(iter);
  }
  auto iter0 = SnyChannelsInactiveTick.find(file_path_md5_value);
  if (iter0 != SnyChannelsInactiveTick.end()) {
    SnyChannelsInactiveTick.erase(iter0);
  }
}

int SNYVODManager::onThreadProc(int id) {
  vector<string> removed_channels;
  while (threads.isRunning(id)) {
    removed_channels.clear();
    vector<string>(removed_channels).swap(removed_channels);
    std::this_thread::sleep_for(std::chrono::milliseconds(10 * 1000));
    auto now = std::chrono::system_clock::now();
    mutex.lock();
    for (const auto &iter : SnyChannelRequestTick) {
      auto last = iter.second;
      std::chrono::duration<double> diff = now - last;
      if (diff.count() > MAX_UNUSED_CHANNEL_DURATION) {
        removed_channels.push_back(iter.first);
      }
    }
    for(const auto &iter : removed_channels) {
      SNYVODChannel* ch = SnyChannels[iter];
      if (SnyChannelsInactive.size() > MAX_CHANNEL_CNT_INACTIVE) {
        std::chrono::system_clock::time_point earlyTick = SnyChannelsInactiveTick.begin()->second;
        std::string channelNeedDel;
        for (const auto & it : SnyChannelsInactiveTick) {
          std::chrono::system_clock::time_point tp = it.second;
          if (tp < earlyTick) {
            channelNeedDel = it.first;
            earlyTick = tp;
          }
        }
        deleteChannelInactive(channelNeedDel);
      }
      
      auto i1 = SnyChannelsInactive.find(iter);
      if (i1 != SnyChannelsInactive.end()) {
        SnyChannelsInactive.erase(iter);
      }
      auto i2 = SnyChannelsInactiveTick.find(iter);
      if (i2 != SnyChannelsInactiveTick.end()) {
        SnyChannelsInactiveTick.erase(iter);
      }
      
      SnyChannelsInactive.insert(make_pair(iter, ch->getUri()));
      SnyChannelsInactiveTick.insert(make_pair(iter, now));
      delete ch; ch = nullptr;
      SnyChannels.erase(SnyChannels.find(iter));
      SnyChannelRequestTick.erase(SnyChannelRequestTick.find(iter));
    }
    mutex.unlock();
  }
  return id;
}

void SNYVODManager::start() {
  threads.start(1);
}

void SNYVODManager::stop() {
  threads.stopAll();
  threads.waitAll();
}


