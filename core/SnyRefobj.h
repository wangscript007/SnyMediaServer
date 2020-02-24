#ifndef _STREAMINGNOLOGY_CORE_REFOBJ_H_
#define _STREAMINGNOLOGY_CORE_REFOBJ_H_

#include <atomic>
#include "typedefine.h"

class SnyRefObj{
public:
  SnyRefObj();
  virtual ~SnyRefObj();
  
public:
  virtual void  addRef();
  virtual void  release();
  virtual int   refCount();
  
protected:
  virtual void  onObjRelease();
  
public:
  static int globalRefCount();
  
private:
  std::atomic<int> mRefCount;
  static std::atomic<int> mGlobalRefCount;
};

#endif //_STREAMINGNOLOGY_CORE_REFOBJ_H_
