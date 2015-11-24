/*
 * String.h
 *
 *  Created on: 26.08.2015
 *      Author: Zalasus
 */

#ifndef INCLUDE_STRING_H_
#define INCLUDE_STRING_H_

#include <string>
#include <vector>

#include "Types.h"

namespace Stoneship
{

	typedef std::string String;

	class StringUtils
	{
	public:

		static uint32_t split(String s, const String &delim, std::vector<String> &elems);

	};

	String operator+(const String &left, uint32_t i);
	String operator+(const String &left, int32_t i);

}

#endif /* INCLUDE_STRING_H_ */
