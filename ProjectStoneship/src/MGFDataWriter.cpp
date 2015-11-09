/*
 * MGFDataWriter.cpp
 *
 *  Created on: Nov 9, 2015
 *      Author: zalasus
 */

#include "MGFDataWriter.h"

namespace Stoneship
{

	MGFDataWriter::MGFDataWriter(std::ostream *stream, MasterGameFile *mgf)
	: mStream(stream),
	  mGameFile(mgf)
	{
	}

	MasterGameFile *MGFDataWriter::getMasterGameFile()
	{
		return mGameFile;
	}

	MGFDataWriter& MGFDataWriter::seek(std::streampos pos)
	{
		if(mStream->eof())
		{
			mStream->clear();
		}

		mStream->seekp(pos);

		return *this;
	}

	std::streampos MGFDataWriter::tell()
	{
		return mStream->tellp();
	}

	MGFDataWriter &MGFDataWriter::writeULong(uint64_t l)
	{
		_writeIntegral<uint64_t>(l);

		return *this;
	}

	MGFDataWriter &MGFDataWriter::writeUInt(uint32_t i)
	{
		_writeIntegral<uint32_t>(i);

		return *this;
	}

	MGFDataWriter &MGFDataWriter::writeUShort(uint16_t s)
	{
		_writeIntegral<uint16_t>(s);

		return *this;
	}

	MGFDataWriter &MGFDataWriter::writeUByte(uint8_t b)
	{
		_writeNext(b);

		return *this;
	}

	MGFDataWriter &MGFDataWriter::writeBString(const String &s)
	{
		size_t len = (s.length() > 0xFF) ? 0xFF : s.length();

		writeUByte(len);

		_writeChars(s.c_str(), len);

		return *this;
	}

	MGFDataWriter &MGFDataWriter::writeSString(const String &s)
	{
		size_t len = (s.length() > 0xFFFF) ? 0xFFFF : s.length();

		writeUShort(len);

		_writeChars(s.c_str(), len);

		return *this;
	}

	MGFDataWriter &MGFDataWriter::writeIString(const String &s)
	{
		size_t len = (s.length() > 0xFFFFFFFF) ? 0xFFFFFFFF : s.length();

		writeUInt(len);

		_writeChars(s.c_str(), len);

		return *this;
	}

	MGFDataWriter &MGFDataWriter::writeZString(const String &s)
	{
		_writeChars(s.c_str(), s.length());

		_writeNext(0);

		return *this;
	}

	void MGFDataWriter::_writeChars(const char *data, size_t size)
	{
		for(uint32_t i = 0; i < size; ++i)
		{
			_writeNext(data[i]);
		}
	}

	void MGFDataWriter::_writeNext(uint8_t c)
	{
		/*if(hasEnded())
		{
			_except(StoneshipException::IO_ERROR, String("Out of writeable stream data (current: ") + (uint32_t)tell() + ", unit end: " + (uint32_t)mUnitEnd + ")");
		}*/

		mStream->put(c);
	}


	template <typename T>
	void MGFDataWriter::_writeIntegral(T v)
	{
		for(uint8_t i = 0; i < sizeof(T); ++i)
		{
			_writeNext(v & 0xFF);
			v = v >> 8;
		}
	}

	void MGFDataWriter::_except(StoneshipException::ExceptionType type, const String &msg)
	{
		//I once was a Java programmer. Can you tell?

		String nmsg(msg);

		if(mGameFile != nullptr)
		{
			nmsg = "In file '" + mGameFile->getFilename() + "': " + nmsg;
		}

		STONESHIP_EXCEPT(type, nmsg);
	}

}

