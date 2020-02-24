#ifndef _STREAMINGNOLOGY_CORE_SNYEVENTCALLBACK_H_
#define _STREAMINGNOLOGY_CORE_SNYEVENTCALLBACK_H_
class SnyIEventCallback {
public:
  SnyIEventCallback() { }
  virtual ~SnyIEventCallback() { }
public:
  virtual void onEventCallback(void* userData) = 0;
};

#endif
