#include "SnyThreads.h"
#include <chrono>
#include <thread>

SThread::SThread() {
  mIsStop    = true;
  mIsRunning = false;
  mThread    = nullptr;
}

SThread::~SThread() { }

int SThread::runThreadLoop(void *user) {
  SThread *pThis = (SThread *)user;
  return pThis->threadLoop();
}

int SThread::threadLoop() {
  m.lock();
  cv.notify_one();
  m.unlock();
  int ret = onThreadLoop();
  mIsRunning = false;
  return ret;
}

int SThread::start(int priority, const char *name) {
  std::unique_lock<std::mutex> lk(m);
  mIsStop = false;
  mIsRunning = true;
  //TODO:reset cv here

  mThread = new thread(SThread::runThreadLoop,this);
  cv.wait(lk);
  lk.unlock();
  return 0;
}

void SThread::stop() {
  mIsStop = true;
}

bool SThread::isStop() {
  return mIsStop;
}

bool SThread::isRunning() {
  return mIsRunning;
}

void SThread::wait() {
  if (mThread != nullptr && mThread->joinable()) {
    mThread->join();
    delete mThread; mThread = nullptr;
  }
  while(mIsRunning) {
    std::this_thread::sleep_for(std::chrono::microseconds(10*1000));
  }
}
