#include "SnySample.h"

SnySample::SnySample() {
  memset(&mFormat, 0, sizeof(mFormat));
  mDts    = 0;
  mPts    = 0;
  mCts    = 0;
  mIsKey  = true;
  mType   = SampleData;
}

SnySample::SnySample(const SnySample &other) {
  memset(&mFormat, 0, sizeof(mFormat));
  mDts    = 0;
  mPts    = 0;
  mCts    = 0;
  mIsKey  = true;
  mType   = SampleData;
  
  operator=(other);
}

SnySample::~SnySample() {
}

int SnySample::type() const {
  return mType;
}

const StreamFormat &SnySample::format() const {
  return mFormat;
}

const char *SnySample::data() const {
  return mDataBuffer.data();
}

int SnySample::size() const {
  return mDataBuffer.size();
}

const SnyDataBuffer &SnySample::dataBuffer() const {
  return mDataBuffer;
}

int64_t SnySample::dts() const {
  return mDts;
}

int64_t SnySample::pts() const {
  return mPts;
}

int64_t SnySample::cts() const {
  return mCts;
}

bool SnySample::isKey() const {
  return mIsKey;
}

const SnyVariantValue &SnySample::attribute(const char *key) const {
  return mAttributes[key];
}

SnyDictionary &SnySample::attributes() {
  return mAttributes;
}

bool SnySample::isEmpty() const {
  return mDataBuffer.isEmpty();
}

void SnySample::setType(int type) {
  this->mType = type;
}

void SnySample::setFormat(const StreamFormat &format) {
  mFormat = format;
}

void SnySample::setDataBuffer(const SnyDataBuffer &dataBuffer) {
  mDataBuffer = dataBuffer;
}

void SnySample::setData(const char *data, int size) {
  mDataBuffer.assign(data, size);
}

void SnySample::setAttribute(const char *key, const SnyVariantValue &value) {
  mAttributes.set(key, value);
}

void SnySample::setAttributes(const SnyDictionary &attributes) {
  mAttributes = attributes;
}

void SnySample::setDts(int64_t dts) {
  mDts = dts;
}

void SnySample::setPts(int64_t pts) {
  mPts = pts;
}

void SnySample::setCts(int64_t cts) {
  mCts = cts;
}

void SnySample::setKey(bool isKey) {
  mIsKey = isKey;
}

void SnySample::operator=(const SnySample &other) {
  mType        = other.mType;
  mFormat      = other.mFormat;
  mDataBuffer  = other.mDataBuffer;
  mDts         = other.mDts;
  mPts         = other.mPts;
  mCts         = other.mCts;
  mIsKey       = other.mIsKey;
  mAttributes  = other.mAttributes;
}
