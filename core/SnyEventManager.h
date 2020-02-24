#ifndef _STREAMINGNOLOGY_CORE_SNYEVENTMANAGER_H_
#define _STREAMINGNOLOGY_CORE_SNYEVENTMANAGER_H_
#include <functional>
#include <thread>

extern "C"{
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>
#include <event2/event_struct.h>
}

class SnyEventManager {
public:
  typedef std::function<void (void* userDatas)> eventCallbackFunc;
  ~SnyEventManager() {}
  static SnyEventManager* getInstance();
  void registerEvent(struct event* et);
  void unregisterEvent(struct event* et);
  static void threadFunc();
private:
  SnyEventManager();
  static SnyEventManager* singleInstance;
  struct event_base* base;
  std::thread    workThread;
};

#endif
