/*
 * Logger.h
 *
 *  Created on: Nov 18, 2015
 *      Author: zalasus
 */

#ifndef INCLUDE_LOGGER_H_
#define INCLUDE_LOGGER_H_

#include "String.h"

namespace Stoneship
{

	class Logger
	{
	public:

		enum LogLevel
		{
			LOGLEVEL_INFO,
			LOGLEVEL_WARNING,
			LOGLEVEL_SEVERE
		};

		Logger(const String &name);

		void log(const String &msg, LogLevel level = LOGLEVEL_INFO);

        String getLoggerName();
        void setEnableTimestamp(bool ts);


        static Logger &getDefaultLogger();

		static inline void info(const String &msg) { getDefaultLogger().log(msg, LOGLEVEL_INFO); }
		static inline void warn(const String &msg) { getDefaultLogger().log(msg, LOGLEVEL_WARNING); }
		static inline void severe(const String &msg) { getDefaultLogger().log(msg, LOGLEVEL_SEVERE); }


	private:

		String mName;
		bool mEnableTimestamp;


		static Logger smDefaultLogger;

	};

}

#endif /* INCLUDE_LOGGER_H_ */
