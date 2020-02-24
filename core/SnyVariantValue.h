#ifndef _STREAMINGNOLOGY_CORE_VARIANTVALUE_H
#define _STREAMINGNOLOGY_CORE_VARIANTVALUE_H
#include "typedefine.h"
#include <string>

class SnyVariantValue {
public :
  SnyVariantValue();
  SnyVariantValue(bool                 value);
  SnyVariantValue(int                  value);
  SnyVariantValue(uint                 value);
  SnyVariantValue(int64_t              value);
  SnyVariantValue(uint64_t             value);
  SnyVariantValue(double               value);
  SnyVariantValue(const char *         value);
  SnyVariantValue(const std::string &       value);
  SnyVariantValue(const void *         value);
  SnyVariantValue(const SnyVariantValue & other);
  
  virtual ~SnyVariantValue();
  
public:
  bool       isValid() const;
  std::string     toString() const;
  
  bool       boolValue      (bool            defaultValue = false)  const;
  int        intValue       (int             defaultValue = 0)      const;
  uint       uintValue      (uint            defaultValue = 0)      const;
  int8_t     int8Value      (int8_t          defaultValue = 0)      const;
  uint8_t    uint8Value     (uint8_t         defaultValue = 0)      const;
  int16_t    int16Value     (int16_t         defaultValue = 0)      const;
  uint16_t   uint16Value    (uint16_t        defaultValue = 0)      const;
  int32_t    int32Value     (int32_t         defaultValue = 0)      const;
  uint32_t   uint32Value    (uint32_t        defaultValue = 0)      const;
  int64_t    int64Value     (int64_t         defaultValue = 0)      const;
  uint64_t   uint64Value    (uint64_t        defaultValue = 0)      const;
  float      floatValue     (float           defaultValue = 0.0f)   const;
  double     doubleValue    (double          defaultValue = 0.0f)   const;
  std::string     stringValue    (const char *    defaultValue = "")     const;
  void *     voidValue      (const void *    defaultValue = NULL)   const;
  
  void operator = (bool                 value);
  void operator = (int                  value);
  void operator = (uint                 value);
  void operator = (int64_t              value);
  void operator = (uint64_t             value);
  void operator = (double               value);
  void operator = (const char *         value);
  void operator = (const std::string &  value);
  void operator = (const void *         value);
  void operator = (const SnyVariantValue & other);
  
protected:
  void init();
  char *allocValue(int size);
  char *copyValue(const char *value, int size);
  void  freeValue();
  
private:
  bool  mIsValid;
  char *mValue;
  int   mSize;
};

#endif // _STREAMINGNOLOGY_CORE_VARIANTVALUE_H
