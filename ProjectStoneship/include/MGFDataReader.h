
#ifndef _MGFDATAREADER_H
#define _MGFDATAREADER_H

#include <istream>

#include "Types.h"
#include "Exception.h"

namespace Stoneship
{

    class MasterGameFile;

	class MGFDataReader
	{
	public:

		MGFDataReader(std::istream *stream, MasterGameFile *mgf = nullptr, uint32_t unitSize = 0);

		MasterGameFile *getMasterGameFile();
		std::istream *getStream();

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
		MGFDataReader &readIntegral(T &v)
		{
            union{ T vt; uint8_t vb[sizeof(T)];} vu;

            for(uint8_t i = 0; i < sizeof(T); ++i)
            {
                //TODO: find a better way to convert to little endian. this might be platform dependent

                vu.vb[i] = _getNext();

                //v |= static_cast<T>(getNext()) << ((sizeof(T)-1)*8);
                //v = v >> 8; // Uhhmm... sure our bytes are always 8 bit long?
            }

            v = vu.vt;

            return *this;
		}

		template <typename T>
		MGFDataReader &readStruct(T &s);

		void beginUnit(uint32_t size);
		void endUnit();
		uint32_t bytesRemainingInUnit();

		void skip(uint32_t amount);
		void skipToEnd();

		bool hasEnded();


		static uint32_t SEEKS;
		static uint32_t TELLS;
		static uint32_t GETCS;


	private:

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
