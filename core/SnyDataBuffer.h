#ifndef _STREAMINGNOLOGY_CORE_DATABUFFER_H_
#define _STREAMINGNOLOGY_CORE_DATABUFFER_H_
#include "SnyRefobj.h"

class SnyDataBuffer {
public:
	SnyDataBuffer();
  SnyDataBuffer(const SnyDataBuffer &dataBuffer);
  SnyDataBuffer(const SnyDataBuffer &dataBuffer, int offset, int length);
  SnyDataBuffer(const char *data, int size);
  SnyDataBuffer(const char *str);
  SnyDataBuffer(int size);
  
  virtual ~SnyDataBuffer();
  
public:
  virtual void         assign(const char *data, int size);
  virtual bool         resize(int size);
  virtual char *       data();
  virtual const char * data() const;
  virtual char *       data(int offset);
  virtual const char * data(int offset) const;
  virtual int          size() const;
  virtual bool         isEmpty() const;
  virtual void         clear();
  virtual void         fill(const char c = 0);
  virtual SnyDataBuffer & subData(const SnyDataBuffer &dataBuffer, int offset, int length);
  virtual bool         append(const char *data, int size);
  virtual bool         append(const SnyDataBuffer &dataBuffer);
  virtual SnyDataBuffer   clone() const;
  virtual SnyDataBuffer & operator = (const SnyDataBuffer &dataBuffer);
  
public:
  static int globalRefCount();
  
private:
  void allocData(const char *data, int size);
  void releaseData();
  
private:
  class  Data;
  Data * mData;
  int    mOffset;
  int    mLength;
  
  static int mGlobalRefCount;
};

#endif // _STREAMINGNOLOGY_CORE_DATABUFFER_H_
