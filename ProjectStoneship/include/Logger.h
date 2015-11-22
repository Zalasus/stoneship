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
			LOGLEVEL_NORMAL,
			LOGLEVEL_WARNING,
			LOGLEVEL_SEVERE
		};

		static void log(const String &msg, LogLevel = LOGLEVEL_NORMAL);
		static void info(const String &msg);
		static void warn(const String &msg);

	};

}

#endif /* INCLUDE_LOGGER_H_ */
