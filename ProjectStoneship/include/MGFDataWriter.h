/*
 * MGFDataWriter.h
 *
 *  Created on: Nov 9, 2015
 *      Author: zalasus
 */

#ifndef INCLUDE_MGFDATAWRITER_H_
#define INCLUDE_MGFDATAWRITER_H_

#include <ostream>

#include "Types.h"
#include "String.h"
#include "Exception.h"

namespace Stoneship
{

    class MasterGameFile;

	class MGFDataWriter
	{
	public:

		MGFDataWriter(std::ostream *out, MasterGameFile *mgf = nullptr);

		MasterGameFile *getMasterGameFile();

		//adapters for seekg/tellg with additional error checking and EOF handling
		MGFDataWriter &seek(std::streampos pos);
		std::streampos tell();

		template <typename T>
        MGFDataWriter &operator<<(const T &x);


	private:

		void _writeNext(uint8_t b);
		void _writeChars(const char *data, size_t size);

		template <typename T>
        void _stupidlyWriteIntegral(T v)
        {
            for(uint8_t i = 0; i < sizeof(T); ++i)
            {
                _writeNext(v & 0xFF);
                v = v >> 8;
            }
        }

		void _except(StoneshipException::ExceptionType type, const String &msg);

		std::ostream *mStream;
		MasterGameFile *mGameFile;

	};

}


#endif /* INCLUDE_MGFDATAWRITER_H_ */
