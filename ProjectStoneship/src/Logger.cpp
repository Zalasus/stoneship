/*
 * Logger.cpp
 *
 *  Created on: Nov 18, 2015
 *      Author: zalasus
 */

#include "Logger.h"

#include <iostream>
#include <ctime>

namespace Stoneship
{

	static const char *getTimestamp()
	{
		std::time_t currentTime = std::time(0);
		std::tm localTime = *std::localtime(&currentTime);

		static char timeString[20];

		std::strftime(timeString, 20 , "%F %T", &localTime);

		return timeString;
	}

	void Logger::log(const String &msg, LogLevel level)
	{

		switch(level)
		{
		case LOGLEVEL_SEVERE:
			std::cerr << "[" << getTimestamp() << "][SEVE] " << msg << std::endl;
			break;

		case LOGLEVEL_WARNING:
			std::cout  << "[" << getTimestamp() <<  "][WARN] " << msg << std::endl;
			break;

		case LOGLEVEL_INFO:
		default:
			std::cout  << "[" << getTimestamp() << "][INFO] " << msg << std::endl;
		}
	}

	void Logger::info(const String &msg)
	{
		log(msg, LOGLEVEL_INFO);
	}

	void Logger::warn(const String &msg)
	{
		log(msg, LOGLEVEL_WARNING);
	}

	void Logger::severe(const String &msg)
	{
	    log(msg, LOGLEVEL_SEVERE);
	}

}


