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
	    #ifdef _DEBUG
	    ++SEEKS; // count total seek operations for debug statistics
	    #endif // _DEBUG

		if(mStream->eof())
		{
			mStream->clear();
		}

		mStream->seekp(pos);

		return *this;
	}

	std::streampos MGFDataWriter::tell()
	{
	    #ifdef _DEBUG
	    ++TELLS; // count total tell operations for debug statistics
	    #endif // _DEBUG

		return mStream->tellp();
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

		#ifdef _DEBUG
		++PUTCS; // count total write operations for debug statistics
		#endif // _DEBUG
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

	uint32_t MGFDataWriter::PUTCS = 0;
	uint32_t MGFDataWriter::SEEKS = 0;
	uint32_t MGFDataWriter::TELLS = 0;

}

