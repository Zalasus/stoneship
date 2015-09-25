/*
 * StoneshipException.cpp
 *
 *  Created on: 27.08.2015
 *      Author: Zalasus
 */


#include "StoneshipException.h"

namespace Stoneship
{

	StoneshipException::StoneshipException(const String& msg)
	: mMsg(msg)
	{
	}

	const char *StoneshipException::what()
	{
		return mMsg.c_str();
	}
}
