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

		static String toLower(const String &s);
		static String toUpper(const String &s);

		static String &ltrim(String &sw);
		static String &rtrim(String &sw);

		static String trim(const String &s);

		static bool startsWith(const String &s, const String &begin);
		static bool endsWith(const String &s, const String &end);

		static bool parseInt(const String &s, int32_t &i);
		static bool parseFloat(const String &s, float &f);

	};

	String operator+(const String &left, uint32_t i);
	String operator+(const String &left, int32_t i);

}

#endif /* INCLUDE_STRING_H_ */
