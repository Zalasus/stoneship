/*
 * MGFDataWriter.cpp
 *
 *  Created on: Nov 9, 2015
 *      Author: zalasus
 */

#include "MGFDataWriter.h"

#include "MasterGameFile.h"

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

	template <>
	MGFDataWriter &MGFDataWriter::operator << <uint64_t>(const uint64_t &l)
	{
		_stupidlyWriteIntegral<uint64_t>(l);

		return *this;
	}

	template <>
	MGFDataWriter &MGFDataWriter::operator << <uint32_t>(const uint32_t &i)
	{
		_stupidlyWriteIntegral<uint32_t>(i);

		return *this;
	}

	template <>
	MGFDataWriter &MGFDataWriter::operator << <uint16_t>(const uint16_t &s)
	{
		_stupidlyWriteIntegral<uint16_t>(s);

		return *this;
	}

	template <>
	MGFDataWriter &MGFDataWriter::operator << <uint8_t>(const uint8_t &b)
	{
		_writeNext(b);

		return *this;
	}

	template <>
	MGFDataWriter &MGFDataWriter::operator << <String>(const String &s)
	{
 		*this << static_cast<uint32_t>(s.length());

		_writeChars(s.c_str(), s.length());

		*this << static_cast<uint8_t>(0);

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
		mStream->put(c);
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

