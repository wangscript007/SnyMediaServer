#ifndef _STREAMINGNOLOGY_CORE_ARGUMENT_H_
#define _STREAMINGNOLOGY_CORE_ARGUMENT_H_

#include "SnyVariantValue.h"
#include <string>

class SnyArgument
{
public:
	SnyArgument(const char *name, const SnyVariantValue &value);
	virtual ~SnyArgument();

public:
  std::string &              name();
	SnyVariantValue  &       value();
	const std::string &        name()  const;
	const SnyVariantValue  & value() const;

	void operator =    (const SnyArgument &other);
	bool operator ==   (const char *name) const;
  std::string             toString() const;
	std::string             toString(const char *kvDelimiter) const;

private:
	std::string        mName;
	SnyVariantValue  mValue;
};

#endif // _STREAMINGNOLOGY_CORE_ARGUMENT_H_
