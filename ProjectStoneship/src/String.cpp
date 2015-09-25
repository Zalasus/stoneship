/*
 * String.cpp
 *
 *  Created on: 27.08.2015
 *      Author: Zalasus
 */

#include "String.h"

#include <sstream>

namespace Stoneship
{


	uint32_t StringUtils::split(String s, const String &delim, std::vector<String> &elems)
	{

		uint32_t i = 0;
		size_t pos = s.find(delim);
		String token;
		while((pos = s.find(delim)) != std::string::npos)
		{
			token = s.substr(0, pos);

			elems.push_back(token);

			s.erase(0, pos + delim.length());

			i++;
		}

		//append stuff after last delimiter
		if(s.length() > 0)
		{
			elems.push_back(s);
			i++;
		}

		return i;
	}


	String operator+(const String &s, uint32_t i)
	{
		std::ostringstream out;

		out << s << i;
		return out.str();
	}

	String operator+(const String &s, int32_t i)
	{
		std::ostringstream out;

		out << s << i;
		return out.str();
	}

}

