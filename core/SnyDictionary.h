#ifndef _STREAMINGNOLOGY_CORE_DICTIONARY_H_
#define _STREAMINGNOLOGY_CORE_DICTIONARY_H_

#include "SnyArgument.h"
#include <vector>
#include <map>

class SnyDictionary {
public:
  SnyDictionary();
  
  virtual ~SnyDictionary();

public:
  SnyArgument &operator[](int index);
  
  SnyArgument &at(int index);
  
  const SnyArgument &operator[](int index) const;
  
  const SnyArgument &at(int index) const;
  
  SnyVariantValue &operator[](const char *name);
  
  SnyVariantValue &value(const char *name);
  
  const SnyVariantValue &operator[](const char *name) const;
  
  const SnyVariantValue &value(const char *name) const;
  
  SnyDictionary &set(const char *name, const SnyVariantValue &value);
  
  bool find(const char *name) const;
  
  bool findValue(const char *name, SnyVariantValue &value) const;
  
  int size() const;
  
  bool empty() const;
  
  bool erase(const char *name);
  
  bool erase(int index);
  
  void clear();
  
  void operator=(const SnyDictionary &other);
  
  SnyDictionary &merge(const SnyDictionary &other);
  
  std::string toString() const;
  
  std::string toString(const char *delimiter, const char *kvDelimiter) const;
  
  std::map<std::string, std::string> toStringMap() const;
  
  std::string toXmlString() const;
  
  bool fromXmlString(const char *xml);

private:
  std::vector<SnyArgument> mValues;
};


#endif // _STREAMINGNOLOGY_CORE_DICTIONARY_H_
