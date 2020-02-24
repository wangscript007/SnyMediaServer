#include "SnyLiveManager.h"

SNYLiveManager::SNYLiveManager() {
  return;
}

SNYLiveManager::~SNYLiveManager() {
  return;
}

void SNYLiveManager::requestContent(Network::IHttpRequestPtr req) {
  return;
}

SNY_Bool SNYLiveManager::createChannel(const std::string channel_uuid) {
  return true;
}

SNY_Bool SNYLiveManager::destroyChannel(const std::string channel_uuid) {
  return true;
}

SNY_Bool SNYLiveManager::startChannel(const std::string channel_uuid) {
  return true;
}

SNY_Bool SNYLiveManager::stopChannel(const std::string channel_uuid) {
  return true;
}

SnyDataBuffer SNYLiveManager::getAllChannelInfo() {
  SnyDataBuffer buf;
  return buf;
}

SnyDataBuffer SNYLiveManager::getChannelInfo(const std::string channel_uuid) {
  SnyDataBuffer buf;
  return buf;
}

void SNYLiveManager::updateLiveChannel(const std::string channel_uuid, const char* config_data, const SNY_SI32 config_length) {
  return;
}

