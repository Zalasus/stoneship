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

	MGFDataReader::MGFDataReader(std::istream *stream, MasterGameFile *mgf, uint32_t unitSize)
	: mStream(stream),
	  mGameFile(mgf),
	  mUnitStart(mStream->tellg()),
	  mUnitEnd(mUnitStart + static_cast<std::streamoff>(unitSize)),
	  mFinite(unitSize)
	{
	}

	MasterGameFile *MGFDataReader::getMasterGameFile()
	{
		return mGameFile;
	}

	MGFDataReader& MGFDataReader::seek(std::streampos pos)
	{
		if(mStream->eof())
		{
			mStream->clear();
		}

		mStream->seekg(pos);

		return *this;
	}

	std::streampos MGFDataReader::tell()
	{
		return mStream->tellg();
	}

	MGFDataReader &MGFDataReader::readULong(uint64_t &l)
	{
		readIntegral<uint64_t>(l);

		return *this;
	}

	MGFDataReader &MGFDataReader::readUInt(uint32_t &i)
	{
		readIntegral<uint32_t>(i);

		return *this;
	}

	MGFDataReader &MGFDataReader::readUShort(uint16_t &s)
	{
		readIntegral<uint16_t>(s);

		return *this;
	}

	MGFDataReader &MGFDataReader::readUByte(uint8_t &b)
	{
		b = getNext();

		return *this;
	}

	//TODO: These functions store the data in the endianess of the platform they are compiled in and NOT in the one advised by KISDF 1.1 This is a heavy problem, as it may prevent us from porting to other systems whilst keeping the data files unchanged. This NEEDS to be fixed
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

	MGFDataReader &MGFDataReader::readBString(String &s)
	{
		uint8_t len;
		readUByte(len);
		char *buf = new char[len];

		for(uint8_t i = 0; i < len; ++i)
		{
			buf[i] = getNext();
		}

		s = String(buf, len);

		delete[] buf;

		return *this;
	}

	MGFDataReader &MGFDataReader::readSString(String &s)
	{
		uint16_t len;
		readUShort(len);
		char *buf = new char[len];

		for(uint16_t i = 0; i < len; ++i)
		{
			buf[i] = getNext();
		}

		s = String(buf, len);

		delete[] buf;

		return *this;
	}

	MGFDataReader &MGFDataReader::readIString(String &s)
	{
		uint32_t len;
		readUInt(len);
		char *buf = new char[len];

		for(uint32_t i = 0; i < len; ++i)
		{
			buf[i] = getNext();
		}

		s = String(buf, len);

		delete[] buf;

		return *this;
	}

	MGFDataReader &MGFDataReader::readZString(String &s)
	{
		std::streamoff off = tell();

		uint32_t len = 0;
		while(getNext() != 0)
		{
			++len;
		}


		seek(off);


		char *buf = new char[len];

		for(uint32_t i = 0; i < len; ++i)
		{
			buf[i] = getNext();
		}

		s = String(buf, len);

		delete[] buf;

		return *this;
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
			throw StoneshipException(String("Tried to read outside of unit bounds (target: ") + (uint32_t)off + ", unit start: " + (uint32_t)mUnitStart + ", unit end: " + (uint32_t)mUnitEnd + ")");
		}
	}

	bool MGFDataReader::hasEnded()
	{
		if(mStream->eof())
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

		return mStream->get();
	}


	template <typename T>
	void MGFDataReader::readIntegral(T &v)
	{
		union{ T vt; uint8_t vb[sizeof(T)];} vu;

		for(uint8_t i = 0; i < sizeof(T); ++i)
		{
			//TODO: find a better way to convert to little endian. this might be platform dependent

			vu.vb[i] = getNext();

			//v |= static_cast<T>(getNext()) << ((sizeof(T)-1)*8);
			//v = v >> 8; // Uhhmm... sure our bytes are always 8 bit long?
		}

		v = vu.vt;

	}


}
