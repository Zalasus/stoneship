/*
 * Record.cpp
 *
 *  Created on: 29.08.2015
 *      Author: Zalasus
 */

#include "Record.h"

#include "MGFDataReader.h"
#include "MasterGameFile.h"
#include "StoneshipException.h"

namespace Stoneship
{

	UID::UID()
	: ordinal(0),
	  id(0)
	{
	}

	UID::UID(Ordinal pOrdinal, ID pId)
	: ordinal(pOrdinal),
	  id(pId)
	{
	}

	UID::UID(uint64_t l)
	{
		ordinal = (l & 0xFFFF00000000) >> 32;
		id = (l & 0xFFFFFFFF);
	}


	//reader function specializations
	template <>
	MGFDataReader &MGFDataReader::readStruct<UID>(UID &uid)
	{
		uid.ordinal = mGameFile.localToGlobalOrdinal(readUShort());
		uid.id = readUInt();

		return *this;
	}

	template <>
	MGFDataReader &MGFDataReader::readStruct<RecordHeader>(RecordHeader &header)
	{
		header.type = readUShort();
		header.dataSize = readUInt();
		header.flags = readUShort();
		header.id = readUInt();

		return *this;
	}

	template <>
	MGFDataReader &MGFDataReader::readStruct<SubrecordHeader>(SubrecordHeader &header)
	{
		header.type = readUShort();
		header.dataSize = readUInt();

		return *this;
	}



	RecordAccessor::RecordAccessor(const RecordHeader &header, std::istream &stream, MasterGameFile &mgf)
	: mHeader(header),
	  mStream(stream),
	  mGameFile(mgf),
	  mInternalReader(mStream, mgf, mHeader.dataSize, true),
	  mOffset(mStream.tellg())
	{
	}

	const RecordHeader &RecordAccessor::getHeader() const
	{
		return mHeader;
	}

	MGFDataReader &RecordAccessor::getReader()
	{
		return mInternalReader;
	}

	MGFDataReader RecordAccessor::getReaderForSubrecord(Record::Subtype subtype)
	{
		while(mInternalReader.bytesRemainingInUnit())
		{
			SubrecordHeader header;
			mInternalReader.readStruct(header);

			if(header.type == subtype) //found fitting subrecord
			{
				return MGFDataReader(mStream, mGameFile, header.dataSize, true, &mInternalReader);

			}else
			{
				mInternalReader.skip(header.dataSize);
			}
		}

		throw StoneshipException("Subrecord not found");
	}

	void RecordAccessor::rollback()
	{
		mInternalReader.seek(mOffset);
	}
}
