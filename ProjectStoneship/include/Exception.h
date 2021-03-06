/*
 * StoneshipException.h
 *
 *  Created on: 27.08.2015
 *      Author: Zalasus
 */

#ifndef INCLUDE_EXCEPTION_H_
#define INCLUDE_EXCEPTION_H_

#include <exception>

#include "String.h"

#ifdef _DEBUG
	#define STONESHIP_EXCEPT(type, msg) throw Stoneship::StoneshipException(type, msg, __FILE__, __LINE__);
#else
	#define STONESHIP_EXCEPT(type, msg) throw Stoneship::StoneshipException(type, msg);
#endif

//TODO: These macros produce exceptions that are far from beeing user readable
#define ASSERT_RECORD_TYPE(expr) if(!(expr)) { STONESHIP_EXCEPT(Stoneship::StoneshipException::INVALID_RECORD_TYPE, "Record type assertion failed: " #expr); }
#define ASSERT_ARG(expr) if(!(expr)) { STONESHIP_EXCEPT(Stoneship::StoneshipException::INVALID_ARGUMENT, "Argument assertion failed: " #expr); }

namespace Stoneship
{

	class StoneshipException : public std::exception
	{
	public:

		enum ExceptionType
		{
			MGF_NOT_FOUND,
			RECORD_NOT_FOUND,
			SUBRECORD_NOT_FOUND,
			ENTITY_ERROR,
			IO_ERROR,
			DATA_FORMAT,
			DEPENDENCY_NOT_MET,
			RESOURCE_ERROR,
			INVALID_RECORD_TYPE,
			RENDERER_ERROR,
			UNSUPPSORTED,
			OUT_OF_BOUNDS,
			INVALID_STATE,
			INVALID_ARGUMENT
		};

		StoneshipException(ExceptionType type, const String &msg);
		StoneshipException(ExceptionType type, const String &msg, const String &file, uint32_t line);

		ExceptionType getType() const;
		const String &getMessage() const;

		const char *what();

	private:

		ExceptionType mType;
		String mMsg;
	};

}


#endif /* INCLUDE_EXCEPTION_H_ */
