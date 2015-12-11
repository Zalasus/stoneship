/*
 * String.cpp
 *
 *  Created on: 27.08.2015
 *      Author: Zalasus
 */

#include "String.h"

#include <sstream>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>

namespace Stoneship
{



    String &StringUtils::ltrim(String &sw)
    {
        sw.erase(sw.begin(), std::find_if(sw.begin(), sw.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));

        return sw;
    }

    String &StringUtils::rtrim(String &sw)
    {
        sw.erase(std::find_if(sw.rbegin(), sw.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), sw.end());

        return sw;
    }

    String StringUtils::trim(const String &s)
    {
        String sw(s);

        ltrim(rtrim(sw));

        return sw;
    }

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

	String StringUtils::toLower(const String &s)
	{
	    return s; //FIXME: Implement this method
	}

	bool StringUtils::startsWith(const String &s, const String &begin)
    {
        if(begin.length() > s.length() || begin.length() == 0 || s.length() == 0)
        {
            return false;
        }

        for(uint32_t i = 0; i < begin.length(); i++)
        {
            if(s[i] != begin[i])
            {
                return false;
            }
        }

        return true;
    }

    bool StringUtils::endsWith(const String &s, const String &end)
    {
        if(end.length() > s.length() || end.length() == 0 || s.length() == 0)
        {
            return false;
        }

        for(uint32_t i = 0; i < end.length(); i++)
        {
            if(s[i + (s.length() - end.length())] != end[i])
            {
                return false;
            }
        }

        return true;
    }

    bool StringUtils::parseInt(const String &s, int32_t &v)
    {
        v = 0;

        for(uint32_t i = 0; i < s.length(); ++i)
        {
            if(isdigit(s[i]))
            {
                v *= 10;
                v += s[i] - '0';

            }else
            {
                return false;
            }
        }

        return true;
    }

    bool StringUtils::parseFloat(const String &s, float &v) //FIXME: Somehow this function does not return totally correct results
    {
        v = 0;
        bool decimals = false;
        float decimal = 1;

        for(uint32_t i = 0; i < s.length(); ++i)
        {
            if(isdigit(s[i]))
            {
                if(!decimals)
                {
                    v *= 10;
                    v += s[i] - '0';

                }else
                {
                    decimal *= 10;
                    v += s[i] / decimal;
                }

            }else if(s[i] == '.' && !decimals)
            {
                decimals = true;

            }else
            {
                return false;
            }
        }

        return true;
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

