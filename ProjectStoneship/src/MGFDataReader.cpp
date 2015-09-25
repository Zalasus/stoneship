/*
 * MGFDataReader.cpp
 *
 *  Created on: 05.07.2014
 *      Author: Zalasus
 */

#include "MGFDataReader.h"

#include "MasterGameFile.h"
#include "StoneshipException.h"
#include "Record.h"

namespace Stoneship
{

	/*static void *memcpy(void *d, const void *s, size_t n)
	{
		const char *sc, *se;
		char *dc;

		dc = (char *) d;
		sc = (const char *) s;
		se = sc + n;

		if(se != sc)
		{
			do
			{
				*dc++ = *sc++;
			}while(sc < se);
		}
		return d;
	}*/

	MGFDataReader::MGFDataReader(std::istream &stream, MasterGameFile &mgf, uint32_t unitSize, bool finite, MGFDataReader *parent)
	: mGameFile(mgf),
	  mStream(stream),
	  mUnitStart(mStream.tellg()),
	  mUnitEnd(mUnitStart + static_cast<std::streamoff>(unitSize)),
	  mFinite(finite),
	  mParent(parent)
	{
	}

	MasterGameFile& MGFDataReader::getMasterGameFile()
	{
		return mGameFile;
	}

	MGFDataReader& MGFDataReader::seek(std::streampos pos)
	{
		if(mStream.eof())
		{
			mStream.clear();
		}

		mStream.seekg(pos);

		return *this;
	}

	std::streampos MGFDataReader::tell()
	{
		return mStream.tellg();
	}

	uint64_t MGFDataReader::readULong()
	{
		uint64_t v = getNext();
		v |= static_cast<uint64_t>(getNext()) << 8; //TODO: replace this crappy implementation with loops or something
		v |= static_cast<uint64_t>(getNext()) << 16;
		v |= static_cast<uint64_t>(getNext()) << 24;
		v |= static_cast<uint64_t>(getNext()) << 32;
		v |= static_cast<uint64_t>(getNext()) << 40;
		v |= static_cast<uint64_t>(getNext()) << 48;
		v |= static_cast<uint64_t>(getNext()) << 56;

		return v;
	}

	uint32_t MGFDataReader::readUInt()
	{
		uint32_t v = getNext();
		v |= static_cast<uint32_t>(getNext()) << 8;
		v |= static_cast<uint32_t>(getNext()) << 16;
		v |= static_cast<uint32_t>(getNext()) << 24;

		return v;
	}

	uint16_t MGFDataReader::readUShort()
	{
		uint16_t v = getNext();
		v |= static_cast<uint16_t>(getNext()) << 8;

		return v;
	}

	uint8_t MGFDataReader::readUByte()
	{
		return getNext();
	}

	/*float MGFDataReader::readFloat()
	{
		float v;
		char data[sizeof(v)];
	#if NRPG_SYSTEM_ENDIANESS == little
		for(int8_t i = 0; i < sizeof(v); --i)
		{
			data[i] = getNext();
		}
	#else
		for(int8_t i = sizeof(v)-1; i >= 0; --i)
		{
			data[i] = getNext();
		}
	#endif

		memcpy(&v, data, sizeof(v));

		return v;
	}

	double MGFDataReader::readDouble()
	{
		double v;
		char data[sizeof(v)];
	#if NRPG_SYSTEM_ENDIANESS == little
		for(int8_t i = 0; i < sizeof(v); ++i)
		{
			data[i] = getNext();
		}
	#else
		for(int8_t i = sizeof(v)-1; i >= 0; --i)
		{
			data[i] = getNext();
		}
	#endif

		memcpy(&v, data, sizeof(v));

		return v;
	}*/

	String MGFDataReader::readBString()
	{
		uint8_t len = readUByte();
		char *buf = new char[len];

		for(uint8_t i = 0; i < len; ++i)
		{
			buf[i] = getNext();
		}

		String v = String(buf, len);

		delete[] buf;

		return v;
	}

	String MGFDataReader::readSString()
	{
		uint16_t len = readUShort();
		char *buf = new char[len];

		for(uint16_t i = 0; i < len; ++i)
		{
			buf[i] = getNext();
		}

		String v = String(buf, len);

		delete[] buf;

		return v;
	}

	String MGFDataReader::readIString()
	{
		uint32_t len = readUInt();
		char *buf = new char[len];

		for(uint32_t i = 0; i < len; ++i)
		{
			buf[i] = getNext();
		}

		String v = String(buf, len);

		delete[] buf;

		return v;
	}

	String MGFDataReader::readZString() //TODO: although terminated strings are discouraged, this method should be made more efficient
	{
		String v;

		unsigned char c;
		while((c = getNext()) != 0)
		{
			v += c;
		}

		return v;
	}

	void MGFDataReader::skip(uint32_t amount)
	{
		std::streamoff current = tell();
		std::streamoff target = current + static_cast<std::streamoff>(amount);

		_checkBounds(target);

		seek(target);
	}


	void MGFDataReader::beginUnit(uint32_t size)
	{
		mUnitStart = tell();
		mUnitEnd = mUnitStart + static_cast<std::streamoff>(size);

		mFinite = true;
	}

	void MGFDataReader::endUnit()
	{
		mFinite = false;
	}

	uint32_t MGFDataReader::bytesRemainingInUnit()
	{
		return mUnitEnd - tell();
	}


	void MGFDataReader::_readChars(char *data, uint32_t size)
	{
		for(uint32_t i = 0; i < size; ++i)
		{
			data[i] = getNext();
		}
	}

	void MGFDataReader::_checkBounds(std::streamoff off)
	{
		if(mFinite && (off > mUnitEnd || off < mUnitStart))
		{
			throw StoneshipException(String("Out of readable stream data (target: ") + (uint32_t)off + ", unit start: " + (uint32_t)mUnitStart + ", unit end: " + (uint32_t)mUnitEnd + ")");
		}
	}

	bool MGFDataReader::hasEnded()
	{
		if(mStream.eof())
		{
			return true;
		}

		_checkBounds(tell());

		return false;
	}

	uint8_t MGFDataReader::getNext()
	{
		if(hasEnded())
		{
			throw StoneshipException(String("Out of readable stream data (current: ") + (uint32_t)tell() + ", unit end: " + (uint32_t)mUnitEnd + ")");
		}

		return mStream.get();
	}

}
