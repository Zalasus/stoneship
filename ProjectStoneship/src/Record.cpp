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

}
