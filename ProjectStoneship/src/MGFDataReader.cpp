/*
 * MGFDataReader.cpp
 *
 *  Created on: 05.07.2014
 *      Author: Zalasus
 */

#include "MGFDataReader.h"

#include "MasterGameFile.h"
#include "Record.h"
#include "Logger.h"

namespace Stoneship
{

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

	std::istream *MGFDataReader::getStream()
	{
	    return mStream;
	}

	MGFDataReader& MGFDataReader::seek(std::streampos pos)
	{
		if(mStream->eof())
		{
			mStream->clear();
		}

		mStream->seekg(pos);

		SEEKS++;

		return *this;
	}

	std::streampos MGFDataReader::tell()
	{
	    TELLS++;
		return mStream->tellg();
	}

	MGFDataReader &MGFDataReader::operator >> (endr_t)
	{
	    skipToEnd();

	    return *this;
	}

	template <>
	MGFDataReader &MGFDataReader::operator >> <uint64_t>(uint64_t &l)
	{
		_stupidlyReadIntegral<uint64_t>(l);

		return *this;
	}

    template <>
	MGFDataReader &MGFDataReader::operator >> <uint32_t>(uint32_t &i)
	{
		_stupidlyReadIntegral<uint32_t>(i);

		return *this;
	}

    template <>
    MGFDataReader &MGFDataReader::operator >> <uint16_t>(uint16_t &s)
	{
		_stupidlyReadIntegral<uint16_t>(s);

		return *this;
	}

    template <>
	MGFDataReader &MGFDataReader::operator >> <uint8_t>(uint8_t &b)
	{
		b = _getNext();

		return *this;
	}

	//FIXME: Portability issue. This could fail if target platform uses different floating point format than the advised IEEE 754
    template <>
	MGFDataReader &MGFDataReader::operator >> <float>(float &f)
	{
		union
		{
			float v;
			char data[sizeof(v)];
		};

		for(uint8_t i = 0; i < sizeof(v); ++i)
		{
			data[i] = _getNext();
		}

		f = v;

		return *this;
	}

	//FIXME: Portability issue. This could fail if target platform uses different floating point format than the advised IEEE 754
    template <>
	MGFDataReader &MGFDataReader::operator >> <double>(double &d)
	{
		union
		{
			double v;
			char data[sizeof(v)];
		};

		for(uint8_t i = 0; i < sizeof(v); ++i)
		{
			data[i] = _getNext();
		}

		d = v;

		return *this;
	}

    template <>
	MGFDataReader &MGFDataReader::operator >> <String>(String &s)
	{
		uint32_t len;
		*this >> len;
		char *buf = new char[len];

		for(uint32_t i = 0; i < len; ++i)
		{
		    uint8_t c = _getNext();

		    if(c == 0)
		    {
		        Logger::warn("Premature termination of string. This indicates an invalid lnegth field or malformed MGF");
		    }

			buf[i] = c;
		}

		if(_getNext() != 0)
		{
		    Logger::warn("Missing termination of string. This indicates an invalid lnegth field or malformed MGF");

		    mStream->unget();
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

	void MGFDataReader::skipToEnd()
	{
	    seek(mUnitEnd);
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
			data[i] = _getNext();
		}
	}

	void MGFDataReader::_checkBounds(std::streamoff off)
	{
		if(mFinite && (off > mUnitEnd || off < mUnitStart))
		{
			_except(StoneshipException::IO_ERROR, String("Tried to read outside of unit bounds (target: ") + (uint32_t)off + ", unit start: " + (uint32_t)mUnitStart + ", unit end: " + (uint32_t)mUnitEnd + ")");
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

	uint8_t MGFDataReader::_getNext()
	{
		if(hasEnded())
		{
			_except(StoneshipException::IO_ERROR, String("Out of readable stream data (current: ") + (uint32_t)tell() + ", unit end: " + (uint32_t)mUnitEnd + ")");
		}

		GETCS++;
		return mStream->get();
	}


	uint32_t MGFDataReader::SEEKS = 0;
    uint32_t MGFDataReader::TELLS = 0;
    uint32_t MGFDataReader::GETCS = 0;

	/* Implementation moved to header for now
	template <typename T>
	MGFDataReader &MGFDataReader::readIntegral(T &v)
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

	}*/

	void MGFDataReader::_except(StoneshipException::ExceptionType type, const String &msg)
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
