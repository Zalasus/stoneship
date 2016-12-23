/*
 * Logger.h
 *
 *  Created on: Nov 18, 2015
 *      Author: zalasus
 */

#ifndef INCLUDE_LOGGER_H_
#define INCLUDE_LOGGER_H_

#include <iostream>

#include "String.h"

namespace Stoneship
{
    class ILoggerListener;

	class Logger
	{
	public:

		enum LogLevel
		{
			LOGLEVEL_INFO,
			LOGLEVEL_WARNING,
			LOGLEVEL_SEVERE,
			LOGLEVEL_DEBUG
		};

		Logger(const String &name, std::ostream *stream = &std::cout);

		void log(const String &msg, LogLevel level = LOGLEVEL_INFO);

        String getLoggerName();
        void setEnableTimestamp(bool ts);
        void setPrintDebug(bool b);
        void setOutputStream(std::ostream *s);
        void setChildLogger(Logger *l);
        void addListener(ILoggerListener *listener);
        void removeListener(ILoggerListener *listener);
        /**
         * Should be 4 characters (looks nice and tidy)
         */
        String getLabelForLevel(LogLevel level);

        static Logger &getDefaultLogger();
		static inline void info(const String &msg) { getDefaultLogger().log(msg, LOGLEVEL_INFO); }
		static inline void warn(const String &msg) { getDefaultLogger().log(msg, LOGLEVEL_WARNING); }
		static inline void severe(const String &msg) { getDefaultLogger().log(msg, LOGLEVEL_SEVERE); }
		static inline void debug(const String &msg) { getDefaultLogger().log(msg, LOGLEVEL_DEBUG); }


	private:

		String mName;
		bool mEnableTimestamp;
		bool mPrintDebugInfo;
		std::ostream *mStream;
		Logger *mChildLogger;
		std::vector<ILoggerListener*> mListeners;


		static Logger smDefaultLogger;

	};
	
	//TODO: Merge child logger and listener interfaces or remove one of them
	class ILoggerListener
	{
    public:
    
        virtual void onLog(const String &msg, Logger::LogLevel level) = 0;
	};

}

#endif /* INCLUDE_LOGGER_H_ */
