#include "SnyDataBuffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
	
#define PADDING_SIZE  16

static char gEmptyData[1] = { 0x00 };

int SnyDataBuffer::mGlobalRefCount = 0;

class SnyDataBuffer::Data : public SnyRefObj{
public:
  Data(const char *data, int size);
  virtual ~Data();
  
public:
  virtual char*        data()                 { return mData; }
  virtual const char*  data() const           { return mData; }
  virtual char*        data(int offset);
  virtual const char*  data(int offset)                         const;
  virtual int          size() const           { return mSize; }
  
public:
  char*  mData;
  int    mSize;
};

SnyDataBuffer::Data::Data(const char *data, int size){
  mSize = 0;
  mData = (char *)malloc(size+PADDING_SIZE);
  SnyDataBuffer::mGlobalRefCount ++;
  if(mData != NULL){
    mSize = size;
    if(data != NULL){
      memcpy(mData, data, size);
    }
    mData[size] = 0x00;
  }
}

SnyDataBuffer::Data::~Data() {
  if(mData != NULL) {
    free(mData);
    mData = NULL;
    SnyDataBuffer::mGlobalRefCount --;
  }
  mSize = 0;
}

char* SnyDataBuffer::Data::data(int offset) {
  if(mData == NULL) {
    return gEmptyData;
  }
  return mData+offset;
}

const char* SnyDataBuffer::Data::data(int offset) const {
  if(mData == NULL)
    return gEmptyData;
  return mData+offset;
}

SnyDataBuffer::SnyDataBuffer() {
  allocData(NULL, 0);
}

SnyDataBuffer::SnyDataBuffer(const SnyDataBuffer &dataBuffer) {
  mData    = NULL;
  mOffset  = -1;
  mLength  = 0;
  operator = (dataBuffer);
}

SnyDataBuffer::SnyDataBuffer(const SnyDataBuffer &dataBuffer, int offset, int length) {
  mData   = NULL;
  mOffset = -1;
  mLength = 0;
  subData(dataBuffer, offset, length);
}

SnyDataBuffer::SnyDataBuffer(const char *data, int size) {
  allocData(data, size);
}

SnyDataBuffer::SnyDataBuffer(const char *str) {
  allocData(str, (int)strlen(str));
}

SnyDataBuffer::SnyDataBuffer(int size) {
  allocData(NULL, size);
}

SnyDataBuffer::~SnyDataBuffer() {
  releaseData();
}

int SnyDataBuffer::globalRefCount() {
  return SnyDataBuffer::mGlobalRefCount;
}

void SnyDataBuffer::allocData(const char *data, int size) {
  mData   = new Data(data, size);
  mOffset = -1;
  mLength = 0;
}

void SnyDataBuffer::releaseData() {
  if(mData != NULL) {
    mData->release();
    mData = NULL;
  }
  mOffset = -1;
  mLength = 0;
}

void SnyDataBuffer::assign(const char *data, int size) {
  releaseData();
  allocData(data, size);
}

bool SnyDataBuffer::resize(int size) {
  releaseData();
  allocData(NULL, size);
  return (!isEmpty());
}

char * SnyDataBuffer::data() {
  if(mData == NULL) {
    return gEmptyData;
  }
  if(mOffset >= 0) {
    return mData->data(mOffset);
  }
  return mData->data();
}

const char * SnyDataBuffer::data() const {
  if(mData == NULL) {
    return gEmptyData;
  }
  if(mOffset >= 0) {
    return mData->data(mOffset);
  }
  return mData->data();
}

char * SnyDataBuffer::data(int offset) {
  return (data() + offset);
}

const char * SnyDataBuffer::data(int offset) const {
  return (data() + offset);
}

int SnyDataBuffer::size() const {
  if(mData == NULL) {
    return 0;
  }
  if(mOffset >= 0) {
    return mLength;
  }
  return mData->size();
}

bool SnyDataBuffer::isEmpty() const {
  return (this->size() <= 0);
}

void SnyDataBuffer::clear() {
  releaseData();
}

void SnyDataBuffer::fill(const char c) {
  memset(data(), c, size());
}

SnyDataBuffer & SnyDataBuffer::subData(const SnyDataBuffer &dataBuffer, int offset, int length) {
  releaseData();
  if(dataBuffer.mData != NULL) {
    dataBuffer.mData->addRef();
    mData   = dataBuffer.mData;
    mOffset = offset;
    mLength = length;
  }
  return (*this);
}

bool SnyDataBuffer::append(const char *oData, int oSize) {
  int newSize = oSize + this->size();
  Data *newData = new Data(NULL, newSize);
  if(newData == NULL) {
    return false;
  }
  if(newData->data() == NULL) {
    newData->release();
    return false;
  }
  int offset = 0;
  if(!this->isEmpty()) {
    memcpy(newData->data(), this->data(), this->size());
    offset += this->size();
  }
  memcpy(newData->data(offset), oData, oSize);
  releaseData();
  mData = newData;
  
  return true;
}

bool SnyDataBuffer::append(const SnyDataBuffer &dataBuffer) {
  return this->append(dataBuffer.data(), dataBuffer.size());
}

SnyDataBuffer SnyDataBuffer::clone() const {
  return SnyDataBuffer(this->data(), this->size());
}

SnyDataBuffer & SnyDataBuffer::operator = (const SnyDataBuffer &dataBuffer) {
  releaseData();
  if(dataBuffer.mData != NULL) {
    dataBuffer.mData->addRef();
    mData   = dataBuffer.mData;
    mOffset = dataBuffer.mOffset;
    mLength = dataBuffer.mLength;
  }
  return (*this);
}
