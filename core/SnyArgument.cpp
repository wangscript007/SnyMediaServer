#include "SnyArgument.h"

// class Argument
SnyArgument::SnyArgument(const char *name, const SnyVariantValue &value)
{
	mName.assign((name == NULL) ? "" : name);
	mValue = value;
}

SnyArgument::~SnyArgument()
{
}

std::string &SnyArgument::name()
{
	return mName;
}

SnyVariantValue &SnyArgument::value()
{
	return mValue;
}

const std::string & SnyArgument::name() const
{
	return mName;
}

const SnyVariantValue & SnyArgument::value() const
{
	return mValue;
}

void SnyArgument::operator = (const SnyArgument &other)
{
	this->mName  = other.mName;
	this->mValue = other.mValue;
}

bool SnyArgument::operator == (const char *name) const
{
	return (name == NULL) ? false : (_stricmp(mName.c_str(), name) == 0);
}

std::string SnyArgument::toString() const
{
    return toString("=");
}

std::string SnyArgument::toString(const char *kvDelimiter) const
{
	std::string str;
	str.reserve(128);
	str += mName;
	str += kvDelimiter;
	str += mValue.toString();
	return str;
}
