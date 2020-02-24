#include "typedefine.h"
#include "SnyVariantValue.h"
#include <stdlib.h>
#include <string.h>

SnyVariantValue::SnyVariantValue() {
	init();
}

void SnyVariantValue::init() {
  mIsValid  = false;
  mValue    = NULL;
  mSize     = 0;
}

SnyVariantValue::SnyVariantValue(bool value) {
  init();
  operator = (value);
}

SnyVariantValue::SnyVariantValue(int value) {
  init();
  operator = (value);
}

SnyVariantValue::SnyVariantValue(uint value) {
  init();
  operator = (value);
}

SnyVariantValue::SnyVariantValue(int64_t value) {
  init();
  operator = (value);
}

SnyVariantValue::SnyVariantValue(uint64_t value) {
  init();
  operator = (value);
}

SnyVariantValue::SnyVariantValue(double value) {
  init();
  operator = (value);
}

SnyVariantValue::SnyVariantValue(const char * value) {
  init();
  operator = (value);
}

SnyVariantValue::SnyVariantValue(const std::string & value) {
  init();
  operator = (value);
}

SnyVariantValue::SnyVariantValue(const void * value) {
  init();
  operator = (value);
}

SnyVariantValue::SnyVariantValue(const SnyVariantValue & other) {
  init();
  operator = (other);
}

SnyVariantValue::~SnyVariantValue() {
  freeValue();
}

bool SnyVariantValue::isValid() const {
  return (mIsValid && (mValue != NULL));
}

std::string SnyVariantValue::toString() const {
  if(!isValid())
    return "";
  return std::string(mValue);
}

bool SnyVariantValue::boolValue(bool defaultValue) const {
  if(!isValid())
    return defaultValue;
  if((_stricmp(mValue, "1") == 0) || (_stricmp(mValue, "true") == 0))
    return true;
  else if((_stricmp(mValue, "0") == 0) || (_stricmp(mValue, "false") == 0))
    return false;
  return defaultValue;
}

int SnyVariantValue::intValue(int defaultValue) const {
  return (int)int64Value(defaultValue);
}

uint SnyVariantValue::uintValue(uint defaultValue) const {
  return (uint)uint64Value(defaultValue);
}

int8_t SnyVariantValue::int8Value(int8_t defaultValue) const {
  return (int8_t)int64Value(defaultValue);
}

uint8_t SnyVariantValue::uint8Value(uint8_t defaultValue) const {
  return (uint8_t)uint64Value(defaultValue);
}

int16_t SnyVariantValue::int16Value(int16_t defaultValue) const {
  return (int16_t)int64Value(defaultValue);
}

uint16_t SnyVariantValue::uint16Value(uint16_t defaultValue) const {
  return (uint16_t)uint64Value(defaultValue);
}

int32_t SnyVariantValue::int32Value(int32_t defaultValue) const {
  return (int32_t)int64Value(defaultValue);
}

uint32_t SnyVariantValue::uint32Value(uint32_t defaultValue) const {
  return (uint32_t)uint64Value(defaultValue);
}

int64_t SnyVariantValue::int64Value(int64_t defaultValue) const {
  if(!isValid())
    return defaultValue;
  int64_t v = defaultValue;
  if(sscanf_s(mValue, "%lld", &v) == -1)
    return defaultValue;
  return v;
}

uint64_t SnyVariantValue::uint64Value(uint64_t defaultValue) const {
  if(!isValid())
    return defaultValue;
  uint64_t v = defaultValue;
  if(sscanf_s(mValue, "%llu", &v) == -1)
    return defaultValue;
  return v;
}

float SnyVariantValue::floatValue(float defaultValue) const {
  return (float)doubleValue(defaultValue);
}

double SnyVariantValue::doubleValue(double defaultValue) const {
  if(!isValid())
    return defaultValue;
  double v = defaultValue;
  if(sscanf_s(mValue, "%lf", &v) == -1)
    return defaultValue;
  return v;
}

std::string SnyVariantValue::stringValue(const char * defaultValue) const {
  if(!isValid())
    return std::string((defaultValue == NULL) ? "" : defaultValue);
  return std::string(mValue);
}

void * SnyVariantValue::voidValue(const void * defaultValue) const {
  if(!isValid())
    return (void *)defaultValue;
  void *v = (void *)defaultValue;
  if(sscanf_s(mValue, "%p", &v) == -1)
    return (void *)defaultValue;
  return v;
}

char *SnyVariantValue::allocValue(int size) {
  freeValue();
  mSize = size+1;
  mValue = (char *)malloc(mSize);
  if(mValue == NULL)
    return NULL;
  memset(mValue, 0, mSize);
  return mValue;
}

char *SnyVariantValue::copyValue(const char *value, int size) {
  freeValue();
  if((value == NULL) || (size <= 0))
    return NULL;
  
  mSize = size;
  mValue = (char *)malloc(mSize);
  if(mValue == NULL)
    return NULL;
  memcpy(mValue, value, mSize);
  return mValue;
}

void SnyVariantValue::freeValue() {
  if(mValue != NULL) {
    free(mValue);
    mValue = NULL;
  }
  mSize = 0;
  mIsValid = false;
}

void SnyVariantValue::operator = (bool value) {
  if(allocValue(1) == NULL)
    return ;
  mValue[0] = value ? '1' : '0';
  mIsValid  = true;
}

void SnyVariantValue::operator = (int value) {
  operator = ((int64_t)value);
}

void SnyVariantValue::operator = (uint value) {
  operator = ((uint64_t)value);
}

void SnyVariantValue::operator = (int64_t value) {
  if(allocValue(64) == NULL)
    return ;
  sprintf_s(mValue, mSize, "%lld", value);
  mIsValid = true;
}

void SnyVariantValue::operator = (uint64_t value) {
  if(allocValue(64) == NULL)
    return ;
  sprintf_s(mValue, mSize, "%llu", value);
  mIsValid = true;
}

void SnyVariantValue::operator = (double value) {
  if(allocValue(64) == NULL)
    return ;
  sprintf_s(mValue, mSize, "%.6f", value);
  mIsValid = true;
}

void SnyVariantValue::operator = (const char * value) {
  if(allocValue(((int)strlen(value))+1) == NULL)
    return ;
  sprintf_s(mValue, mSize, "%s", value);
  mIsValid = true;
}

void SnyVariantValue::operator = (const std::string & value) {
  operator = (value.c_str());
}

void SnyVariantValue::operator = (const void * value) {
  if(allocValue(64) == NULL)
    return ;
  sprintf_s(mValue, mSize, "%p", value);
  mIsValid = true;
}

void SnyVariantValue::operator = (const SnyVariantValue & other) {
  this->copyValue(other.mValue, other.mSize);
  this->mIsValid  = other.mIsValid;
}
