
#ifndef _MGFDATAREADER_H
#define _MGFDATAREADER_H

#include <istream>

#include "Util.h"
#include "String.h"
#include "StoneshipException.h"

namespace Stoneship
{

	class MasterGameFile;

	class MGFDataReader
	{
	public:

		MGFDataReader(std::istream *stream, MasterGameFile *mgf = nullptr, uint32_t unitSize = 0);

		MasterGameFile *getMasterGameFile();

		//adapters for seekg/tellg with additional error checking and EOF handling
		MGFDataReader &seek(std::streampos pos);
		std::streampos tell();

		MGFDataReader &readULong(uint64_t &l);
		MGFDataReader &readLong(int64_t &l);

		MGFDataReader &readUInt(uint32_t &i);
		MGFDataReader &readInt(int32_t &i);

		MGFDataReader &readUShort(uint16_t &s);
		MGFDataReader &readShort(int16_t &s);

		MGFDataReader &readUByte(uint8_t &b);
		MGFDataReader &readByte(int8_t &b);

		MGFDataReader &readFloat(float &f);
		MGFDataReader &readDouble(double &d);

		MGFDataReader &readBString(String &s);
		inline MGFDataReader &readTString(String &s) {return readBString(s);} //supplied for KISDF 1.0 conformity

		MGFDataReader &readSString(String &s);

		MGFDataReader &readIString(String &s);
		inline MGFDataReader &readString(String &s) {return readIString(s);}  //supplied for KISDF 1.0 conformity

		MGFDataReader &readZString(String &s);


		template <typename T>
		MGFDataReader &readStruct(T &s);


		void beginUnit(uint32_t size);
		void endUnit();
		uint32_t bytesRemainingInUnit();

		void skip(uint32_t amount);

		bool hasEnded();


	private:

		template <typename T>
		void _readIntegral(T &v);

		void _readChars(char* data, uint32_t size);

		void _checkBounds(std::streamoff off);

		void _except(StoneshipException::ExceptionType type, const String &msg);

		uint8_t _getNext();

		std::istream *mStream;
		MasterGameFile *mGameFile;

		std::streamoff mUnitStart;
		std::streamoff mUnitEnd;
		bool mFinite;
	};

}


#endif
