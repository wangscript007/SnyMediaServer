#ifndef _STREAMINGNOLOGY_CORE_SNYTHREADS_H_
#define _STREAMINGNOLOGY_CORE_SNYTHREADS_H_

#include <thread>
#include <map>
#include <mutex>
#include <condition_variable>

using namespace std;
class SThread {
public:
  SThread();
  virtual ~SThread();
  
public:
  int   start(int priority = 0, const char *name = "");
  void  stop();
  bool  isStop();
  bool  isRunning();
  void  wait();
  
protected:
  virtual int onThreadLoop() = 0;
  
private:
  static  int runThreadLoop(void *user);
  virtual int threadLoop();
  
private:
  bool    mIsStop;
  bool    mIsRunning;
  std::mutex    m;
  std::condition_variable cv;
  std::thread  *mThread;
};

template<class ThreadHandler>
class Threads {
public:
  Threads(ThreadHandler *handler) : mHandler(handler) { }
  virtual ~Threads() { }

public:
  bool  start(int id, int priority = 0, const char *name = "");
  void  stop(int id);
  bool  isStart(int id);
  void  stopAll();
  bool  isStop(int id);
  bool  isRunning(int id);
  void  wait(int id);
  void  waitAll();

protected:
  class AThread : public SThread {
  public:
    AThread(Threads *pt, int id) : SThread(), mPt(pt), mId(id) { }
    int   start(int priority = 0, const char *name = "") { return SThread::start(priority, name); }
    void  stop()   { SThread::stop(); }
    bool  isStop() { return SThread::isStop(); }
    void  wait()   { SThread::wait(); }
  private:
    Threads   *mPt;
    int        mId;
    virtual int onThreadLoop() { return mPt->onThreadProc(mId); }
  };
  int onThreadProc(int id) { return mHandler->onThreadProc(id); }
  
private:
  ThreadHandler         *mHandler;
  map<int, AThread *>    mThreads;
  std::recursive_mutex   mutex;
};

template<class ThreadHandler>
bool Threads<ThreadHandler>::start(int id, int priority, const char *name) {
  std::lock_guard<std::recursive_mutex> lock(mutex);
  
  if(mThreads.find(id) != mThreads.end()) {
    return true;
  }
  
  AThread *pThread = new AThread(this, id);
  if(pThread == NULL) {
    return false;
  }
  mThreads[id] = pThread;
  if(pThread->start(priority, name) < 0) {
    mThreads.erase(id);
    delete pThread;
    return false;
  }
  return true;
}

template<class ThreadHandler>
void Threads<ThreadHandler>::stop(int id) {
  std::lock_guard<std::recursive_mutex> lock(mutex);
  if(mThreads.find(id) == mThreads.end()) {
    return ;
  }
  
  AThread *pThread = mThreads[id];
  pThread->stop();
}

template<class ThreadHandler>
bool Threads<ThreadHandler>::isStart(int id) {
  std::lock_guard<std::recursive_mutex> lock(mutex);
  if(mThreads.find(id) != mThreads.end()) {
    return true;
  }
  return false;
}

template<class ThreadHandler>
void Threads<ThreadHandler>::stopAll() {
  std::lock_guard<std::recursive_mutex> lock(mutex);
  for(typename map<int, AThread *>::iterator iter = mThreads.begin(); iter != mThreads.end(); iter ++) {
    AThread *pThread = iter->second;
    pThread->stop();
  }
}

template<class ThreadHandler>
bool Threads<ThreadHandler>::isStop(int id) {
  std::lock_guard<std::recursive_mutex> lock(mutex);
  if(mThreads.find(id) == mThreads.end()) {
    return true;
  }
  AThread *pThread = mThreads[id];
  return pThread->isStop();
}

template<class ThreadHandler>
bool Threads<ThreadHandler>::isRunning(int id) {
  std::lock_guard<std::recursive_mutex> lock(mutex);
  if (mThreads.find(id) == mThreads.end()) {
    return false;
  }
  AThread *pThread = mThreads[id];
  return pThread->isRunning();
}

template<class ThreadHandler>
void Threads<ThreadHandler>::wait(int id) {
  mutex.lock();
  if(mThreads.find(id) == mThreads.end()) {
    mutex.unlock();
    return ;
  }
  AThread *pThread = mThreads[id];
  mThreads.erase(id);
  mutex.unlock();
  
  pThread->wait();
  delete pThread;
}

template<class ThreadHandler>
void Threads<ThreadHandler>::waitAll() {
  while(true) {
    mutex.lock();
    if(mThreads.empty()) {
      mutex.unlock();
      break ;
    }
    typename map<int, AThread *>::iterator iter = mThreads.begin();
    AThread *pThread = iter->second;
    mThreads.erase(iter);
    
    mutex.unlock();
    
    pThread->wait();
    delete pThread;
  }
}

#endif
