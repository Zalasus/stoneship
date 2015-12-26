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

	Logger::Logger(const String &name)
	: mName(name),
	  mEnableTimestamp(true)
	{
	}

	void Logger::log(const String &msg, LogLevel level)
	{

	    if(mEnableTimestamp)
	    {
	        std::cout << "[" << getTimestamp() << "]";
	    }

		switch(level)
		{
		case LOGLEVEL_SEVERE:
			std::cout << "[SEVE] " << msg << std::endl;
			break;

		case LOGLEVEL_WARNING:
			std::cout << "[WARN] " << msg << std::endl;
			break;

		case LOGLEVEL_INFO:
		default:
			std::cout << "[INFO] " << msg << std::endl;
		}
	}

	String Logger::getLoggerName()
	{
	    return mName;
	}

    void Logger::setEnableTimestamp(bool ts)
    {
        mEnableTimestamp = ts;
    }


    Logger Logger::smDefaultLogger("Default");

    Logger &Logger::getDefaultLogger()
    {
        return smDefaultLogger;
    }

}


