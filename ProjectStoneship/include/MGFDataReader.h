
#ifndef _MGFDATAREADER_H
#define _MGFDATAREADER_H

#include <istream>

#include "String.h"

namespace Stoneship
{

	class MasterGameFile;

	class MGFDataReader
	{
	public:

		MGFDataReader(std::istream &stream, MasterGameFile &mgf, uint32_t unitSize = 0, bool finite = false, MGFDataReader *parent = nullptr);

		MasterGameFile &getMasterGameFile();

		//adapters for seekg/tellg with additional error checking and EOF handling
		MGFDataReader &seek(std::streampos pos);
		std::streampos tell();
	
		uint64_t readULong();
		inline int64_t readLong() {return static_cast<int64_t>(readULong());} //TODO: replace these casts with real methods that read signed types (portability!)

		uint32_t readUInt();
		inline int32_t readInt() {return static_cast<int32_t>(readUInt());}

		uint16_t readUShort();
		inline int16_t readShort() {return static_cast<int16_t>(readUShort());}

		uint8_t readUByte();
		inline int8_t readByte() {return static_cast<int8_t>(readUByte());}


		//TODO: These functions store the data in the endianess of the platform they are compiled in and NOT in the one advised by KISDF 1.1 This is a heavy problem, as it may prevent us from porting to other systems whilst keeping the data files unchanged. This NEEDS to be fixed
		float readFloat();
		double readDouble();

		String readBString();
		inline String readTString() {return readBString();} //supplied for KISDF 1.0 conformity

		String readSString();

		String readIString();
		inline String readString() {return readIString();}  //supplied for KISDF 1.0 conformity

		String readZString();


		template <typename T>
		MGFDataReader &readStruct(T &s);


		void beginUnit(uint32_t size);
		void endUnit();
		uint32_t bytesRemainingInUnit();

		void skip(uint32_t amount);

		bool hasEnded();

		void _readChars(char* data, uint32_t size);

		void _checkBounds(std::streamoff off);

	private:

		uint8_t getNext();

		MasterGameFile &mGameFile;
		std::istream &mStream;

		std::streamoff mUnitStart;
		std::streamoff mUnitEnd;
		bool mFinite; //a finite reader can't read more bytes than given by unit size

		MGFDataReader *mParent;

	};

}


#endif
