/*
 * StoneshipException.cpp
 *
 *  Created on: 27.08.2015
 *      Author: Zalasus
 */


#include "StoneshipException.h"

#ifdef _DEBUG
	#include <sstream>
#endif

namespace Stoneship
{

	StoneshipException::StoneshipException(ExceptionType type, const String& msg)
	: mType(type),
	  mMsg(msg)
	{
	}

#ifdef _DEBUG
	StoneshipException::StoneshipException(ExceptionType type, const String& msg, const String &file, uint32_t line)
	: mType(type),
	  mMsg(msg)
	{
		std::ostringstream o;
		o << mMsg << " (in line " << line << " of file '" << file << "')";
		mMsg = o.str();
	}
#endif

	StoneshipException::ExceptionType StoneshipException::getType() const
	{
		return mType;
	}

	const String &StoneshipException::getMessage() const
	{
		return mMsg;
	}

	const char *StoneshipException::what()
	{
		return mMsg.c_str();
	}
}
