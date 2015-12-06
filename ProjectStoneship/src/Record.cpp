/*
 * Record.cpp
 *
 *  Created on: 29.08.2015
 *      Author: Zalasus
 */

#include <Exception.h>
#include "Record.h"

#include "MGFDataReader.h"
#include "MasterGameFile.h"

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

	bool UID::operator==(const UID &right)
	{
		return id == right.id && ordinal == right.ordinal;
	}


	//reader function specializations
	template <>
	MGFDataReader &MGFDataReader::readStruct<UID>(UID &uid)
	{
		readUShort(uid.ordinal);

		if(mGameFile != nullptr)
		{
			uid.ordinal = mGameFile->localToGlobalOrdinal(uid.ordinal);

		}

		readUInt(uid.id);

		return *this;
	}

	template <>
	MGFDataReader &MGFDataReader::readStruct<RecordHeader>(RecordHeader &header)
	{
		readUShort(header.type);
		readUInt(header.dataSize);

		if(header.type != Record::TYPE_GROUP)
		{
			readUShort(header.flags);
			readUInt(header.id);

		}else
		{
			readUShort(header.groupType);
			readUInt(header.recordCount);
		}

		return *this;
	}

	template <>
	MGFDataReader &MGFDataReader::readStruct<SubrecordHeader>(SubrecordHeader &header)
	{
		readUShort(header.type);
		readUInt(header.dataSize);

		return *this;
	}



	RecordAccessor::RecordAccessor(const RecordHeader &header, std::istream *stream, MasterGameFile *mgf)
	: mHeader(header),
	  mStream(stream),
	  mGameFile(mgf),
	  mInternalReader(mStream, mgf, mHeader.dataSize),
	  mDataOffset(mStream->tellg())
	{
	}

	RecordAccessor::RecordAccessor(const RecordAccessor &a)
	: mHeader(a.mHeader),
	  mStream(a.mStream),
	  mGameFile(a.mGameFile),
	  mInternalReader(mStream, mGameFile, mHeader.dataSize),
	  mDataOffset(a.mDataOffset)
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
				return MGFDataReader(mStream, mGameFile, header.dataSize);

			}else
			{
				mInternalReader.skip(header.dataSize);
			}
		}

		STONESHIP_EXCEPT(StoneshipException::SUBRECORD_NOT_FOUND, "Subrecord not found");
	}

	MasterGameFile *RecordAccessor::getGameFile()
	{
		return mGameFile;
	}

	std::streampos RecordAccessor::getDataOffset()
	{
		return mDataOffset;
	}

	std::streampos RecordAccessor::getOffset()
	{
		return static_cast<uint32_t>(mDataOffset) - RecordHeader::SIZE_IN_FILE;
	}

	RecordAccessor RecordAccessor::getNextRecord()
	{
		mStream->seekg(static_cast<uint32_t>(mDataOffset) + mHeader.dataSize);
		MGFDataReader ds(mStream, mGameFile);

		RecordHeader header;
		ds.readStruct(header);

		return RecordAccessor(header, mStream, mGameFile);

	}

	RecordAccessor RecordAccessor::getFirstChildRecord()
	{
		if(mHeader.type != Record::TYPE_GROUP)
		{
			STONESHIP_EXCEPT(StoneshipException::DATA_FORMAT, "Tried to access child records of non-group record");
		}

		rollback();

		RecordHeader header;
		mInternalReader.readStruct(header);

		return RecordAccessor(header, mStream, mGameFile);
	}

	/* Implementation removed. This method is deprecated, so the linker should tell anyone.
	RecordAccessor RecordAccessor::getNextChildRecord()
	{
		RecordHeader header;
		mInternalReader.readStruct(header);

		return RecordAccessor(header, mStream, mGameFile);
	}*/

	void RecordAccessor::rollback()
	{
		mInternalReader.seek(mDataOffset);
	}

	void RecordAccessor::skip()
	{
		mStream->seekg(static_cast<uint32_t>(mDataOffset) + mHeader.dataSize);
	}



	IRecordLoadable::~IRecordLoadable()
	{
	}

	void IRecordLoadable::modifyFromRecord(RecordAccessor rec)
	{
	    STONESHIP_EXCEPT(StoneshipException::UNSUPPSORTED, "Loadable does not support modification by record");
	}

}
