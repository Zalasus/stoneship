/*
 * Logger.cpp
 *
 *  Created on: Nov 18, 2015
 *      Author: zalasus
 */

#include "Logger.h"

#include <ctime>
#include <algorithm>

#include "Exception.h"

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
	  mEnableTimestamp(false),
	  mPrintDebugInfo(false),
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

	    if(!mPrintDebugInfo && level == LOGLEVEL_DEBUG)
	    {
	        return;
	    }

	    if(mEnableTimestamp)
	    {
	        *mStream << "[" << getTimestamp() << "]";
	    }

        *mStream << "[" << getLabelForLevel(level) << "] " << msg << std::endl;
        
        for(uint32_t i = 0; i < mListeners.size(); ++i)
        {
            mListeners[i]->onLog(msg, level);
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

    void Logger::setPrintDebug(bool b)
    {
        mPrintDebugInfo = b;
    }

    void Logger::setOutputStream(std::ostream *s)
    {
        mStream = s;
    }

    void Logger::setChildLogger(Logger *l)
    {
        mChildLogger = l;
    }
    
    String Logger::getLabelForLevel(LogLevel level)
    {
        switch(level)
		{
		case LOGLEVEL_SEVERE:
		    return "SEVE";

		case LOGLEVEL_WARNING:
		    return "WARN";

		case LOGLEVEL_DEBUG:
		    return "DBUG";

		case LOGLEVEL_INFO:
		    return "INFO";
		    
		default:
		    return "????";
		}
    }

    void Logger::addListener(ILoggerListener *listener)
    {
        ASSERT_ARG(listener != nullptr);
        
        mListeners.push_back(listener);
    }
    
    void Logger::removeListener(ILoggerListener *listener)
    {
        ASSERT_ARG(listener != nullptr);
        
        std::vector<ILoggerListener*>::iterator it = std::find(mListeners.begin(), mListeners.end(), listener);
        
        if(it != mListeners.end())
        {
            mListeners.erase(it);
        }
    }

    Logger Logger::smDefaultLogger("Default");

    Logger &Logger::getDefaultLogger()
    {
        return smDefaultLogger;
    }

}


