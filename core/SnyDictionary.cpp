#include "SnyDictionary.h"

static SnyVariantValue gEmptyValue;

SnyDictionary::SnyDictionary() {
  mValues.reserve(10);
}

SnyDictionary::~SnyDictionary() {
}

int SnyDictionary::size() const {
  return (int) mValues.size();
}

bool SnyDictionary::empty() const {
  return mValues.empty();
}

bool SnyDictionary::erase(const char *name) {
  bool bOk = false;
  for (int i = 0; i < (int) mValues.size(); i++) {
    if (mValues[i] == name) {
      mValues.erase(mValues.begin() + i);
      bOk = true;
      break;
    }
  }
  return bOk;
}

bool SnyDictionary::erase(int index) {
  if ((index < 0) || (index >= ((int) mValues.size())))
    return false;
  mValues.erase(mValues.begin() + index);
  return true;
}

bool SnyDictionary::find(const char *name) const {
  for (int i = 0; i < (int) mValues.size(); i++) {
    if (mValues[i] == name)
      return true;
  }
  return false;
}

bool SnyDictionary::findValue(const char *name, SnyVariantValue &value) const {
  for (int i = 0; i < (int) mValues.size(); i++) {
    if (mValues[i] == name) {
      value = mValues[i].value();
      return true;
    }
  }
  return false;
}

SnyArgument &SnyDictionary::operator[](int index) {
  return mValues[index];
}

SnyArgument &SnyDictionary::at(int index) {
  return operator[](index);
}

const SnyArgument &SnyDictionary::operator[](int index) const {
  return mValues[index];
}

const SnyArgument &SnyDictionary::at(int index) const {
  return mValues[index];
}

SnyVariantValue &SnyDictionary::operator[](const char *name) {
  for (int i = 0; i < (int) mValues.size(); i++) {
    if (mValues[i] == name)
      return mValues[i].value();
  }
  mValues.push_back(SnyArgument(name, SnyVariantValue()));
  return mValues.back().value();
}

SnyVariantValue &SnyDictionary::value(const char *name) {
  return operator[](name);
}

const SnyVariantValue &SnyDictionary::operator[](const char *name) const {
  for (int i = 0; i < (int) mValues.size(); i++) {
    if (mValues[i] == name)
      return mValues[i].value();
  }
  return gEmptyValue;
}

const SnyVariantValue &SnyDictionary::value(const char *name) const {
  for (int i = 0; i < (int) mValues.size(); i++) {
    if (mValues[i] == name)
      return mValues[i].value();
  }
  return gEmptyValue;
}

SnyDictionary &SnyDictionary::set(const char *name, const SnyVariantValue &value) {
  operator[](name) = value;
  return *this;
}

void SnyDictionary::clear() {
  mValues.clear();
}

void SnyDictionary::operator=(const SnyDictionary &other) {
  this->mValues = other.mValues;
}

SnyDictionary &SnyDictionary::merge(const SnyDictionary &other) {
  for (int i = 0; i < other.size(); i++) {
    set(other[i].name().c_str(), other[i].value());
  }
  return *this;
}

std::string SnyDictionary::toString() const {
  return toString(";", "=");
}

std::string SnyDictionary::toString(const char *delimiter, const char *kvDelimiter) const {
  std::string str;
  str.reserve(128 * 10);
  for (int i = 0; i < (int) mValues.size(); i++) {
    if (i > 0) {
      str += delimiter;
    }
    str += mValues[i].toString(kvDelimiter);
  }
  return str;
}

std::map<std::string, std::string> SnyDictionary::toStringMap() const {
  std::map<std::string, std::string> mapData;
  for (int i = 0; i < (int) mValues.size(); i++) {
    mapData[mValues[i].name()] = mValues[i].value().stringValue();
  }
  return mapData;
}

std::string SnyDictionary::toXmlString() const {
  return "";
}

bool SnyDictionary::fromXmlString(const char *xml) {
  return true;
}
