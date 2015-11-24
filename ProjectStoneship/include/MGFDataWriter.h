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

		MGFDataWriter &writeULong(uint64_t l);
		MGFDataWriter &writeLong(int64_t l);

		MGFDataWriter &writeUInt(uint32_t i);
		MGFDataWriter &writeInt(int32_t i);

		MGFDataWriter &writeUShort(uint16_t s);
		MGFDataWriter &writeShort(int16_t s);

		MGFDataWriter &writeUByte(uint8_t b);
		MGFDataWriter &writeByte(int8_t b);

		MGFDataWriter &writeFloat(float f);
		MGFDataWriter &writeDouble(double d);

		MGFDataWriter &writeBString(const String &s);
		inline MGFDataWriter &writeTString(const String &s) {return writeBString(s);} //supplied for KISDF 1.0 conformity

		MGFDataWriter &writeSString(const String &s);

		MGFDataWriter &writeIString(const String &s);
		inline MGFDataWriter &writeString(const String &s) {return writeIString(s);}  //supplied for KISDF 1.0 conformity

		MGFDataWriter &writeZString(const String &s);


	private:

		void _writeNext(uint8_t b);
		void _writeChars(const char *data, size_t size);

		template <typename T>
		void _writeIntegral(T v);

		void _except(StoneshipException::ExceptionType type, const String &msg);

		std::ostream *mStream;
		MasterGameFile *mGameFile;

	};

}


#endif /* INCLUDE_MGFDATAWRITER_H_ */
