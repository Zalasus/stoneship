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
	  mUnitStart(mStream->tellg()), //FIXME: IO Operation in constructor is a no no!!!
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

		#ifdef _DEBUG
		SEEKS++; // count total seek operations for debug statistics
        #endif  // _DEBUG
        
		return *this;
	}

	std::streampos MGFDataReader::tell()
	{
	    #ifdef _DEBUG
	    TELLS++; // count total tell operations for debug statistics
	    #endif // _DEBUG
        
		return mStream->tellg();
	}

	/**
	 * Specialization enabling users to conveniently end a subrecord reading statement with [...] >> MGFDataReader::endr;
	 * (similar to std::endl)
	 */ 
	MGFDataReader &MGFDataReader::operator >> (endr_t)
	{
	    skipToEnd();

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

		#ifdef _DEBUG
		GETCS++; // count total read operations for debug statistics
		#endif // _DEBUG
		
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
