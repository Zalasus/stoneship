/*
 * StoneshipException.h
 *
 *  Created on: 27.08.2015
 *      Author: Zalasus
 */

#ifndef INCLUDE_STONESHIPEXCEPTION_H_
#define INCLUDE_STONESHIPEXCEPTION_H_

#include <exception>

#include "String.h"

namespace Stoneship
{

	class StoneshipException : public std::exception
	{
	public:
		StoneshipException(const String &msg);

		const char *what();

	private:

		String mMsg;
	};

}


#endif /* INCLUDE_STONESHIPEXCEPTION_H_ */
