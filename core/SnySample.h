#ifndef _STREAMINGNOLOGY_CORE_SAMPLE_H_
#define _STREAMINGNOLOGY_CORE_SAMPLE_H_

#include "typedefine.h"
#include "SnyDataBuffer.h"
#include "SnyDictionary.h"
#include "SnyMediatype.h"

class SnySample {
public:
  SnySample();
  
  SnySample(const SnySample &other);
  
  virtual ~SnySample();

public:
  int type() const;
  
  const StreamFormat &format() const;
  
  const char *data() const;
  
  int size() const;
  
  const SnyDataBuffer &dataBuffer() const;
  
  int64_t dts() const;
  
  int64_t pts() const;
  
  int64_t cts() const;
  
  bool isKey() const;
  
  const SnyVariantValue &attribute(const char *key) const;
  
  SnyDictionary &attributes();
  
  bool isEmpty() const;
  
  void setType(int type);
  
  void setFormat(const StreamFormat &format);
  
  void setDataBuffer(const SnyDataBuffer &dataBuffer);
  
  void setData(const char *data, int size);
  
  void setAttribute(const char *key, const SnyVariantValue &value);
  
  void setAttributes(const SnyDictionary &attributes);
  
  void setDts(int64_t dts);
  
  void setPts(int64_t pts);
  
  void setCts(int64_t cts);
  
  void setKey(bool isKey);
  
  void operator=(const SnySample &other);

private:
  int mType;
  StreamFormat mFormat;
  SnyDataBuffer mDataBuffer;
  int64_t mDts;
  int64_t mPts;
  int64_t mCts;
  bool mIsKey;
  SnyDictionary mAttributes;
};

#endif // _STREAMINGNOLOGY_CORE_SAMPLE_H_
