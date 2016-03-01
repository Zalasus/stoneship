/*
 * Logger.cpp
 *
 *  Created on: Nov 18, 2015
 *      Author: zalasus
 */

#include "Logger.h"

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

	Logger::Logger(const String &name, std::ostream *stream)
	: mName(name),
	  mEnableTimestamp(true),
	  mStream(stream),
	  mChildLogger(nullptr)
	{
	}

	void Logger::log(const String &msg, LogLevel level)
	{
	    if(mChildLogger != nullptr)
	    {
	        mChildLogger->log(msg, level);
	    }

	    if(mStream == nullptr || !mStream->good())
	    {
	        return;
	    }

	    if(mEnableTimestamp)
	    {
	        *mStream << "[" << getTimestamp() << "]";
	    }

		switch(level)
		{
		case LOGLEVEL_SEVERE:
		    *mStream << "[SEVE] " << msg << std::endl;
			break;

		case LOGLEVEL_WARNING:
		    *mStream << "[WARN] " << msg << std::endl;
			break;

		case LOGLEVEL_DEBUG:
		    *mStream << "[DBUG] " << msg << std::endl;
		    break;

		case LOGLEVEL_INFO:
		default:
		    *mStream << "[INFO] " << msg << std::endl;
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

    void Logger::setOutputStream(std::ostream *s)
    {
        mStream = s;
    }

    void Logger::setChildLogger(Logger *l)
    {
        mChildLogger = l;
    }


    Logger Logger::smDefaultLogger("Default");

    Logger &Logger::getDefaultLogger()
    {
        return smDefaultLogger;
    }

}


