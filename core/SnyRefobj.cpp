#include "SnyRefobj.h"

std::atomic<int> SnyRefObj::mGlobalRefCount(0);

SnyRefObj::SnyRefObj(){
  mRefCount = 1;
  SnyRefObj::mGlobalRefCount++;
}

SnyRefObj::~SnyRefObj(){
}

void SnyRefObj::addRef(){
  mRefCount++;
  SnyRefObj::mGlobalRefCount ++;
}

void SnyRefObj::release(){
  mRefCount--;
  SnyRefObj::mGlobalRefCount --;
  if(mRefCount == 0){
    onObjRelease();
    return ;
  }
}

int SnyRefObj::refCount(){
  return mRefCount;
}

void SnyRefObj::onObjRelease(){
  delete this;
}

int SnyRefObj::globalRefCount(){
  return mGlobalRefCount;
}
