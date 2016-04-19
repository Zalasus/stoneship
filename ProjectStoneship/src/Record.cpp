/*
 * Record.cpp
 *
 *  Created on: 29.08.2015
 *      Author: Zalasus
 */

#include "Record.h"

#include "MGFDataReader.h"
#include "MGFDataWriter.h"
#include "Exception.h"
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

	const UID UID::NO_UID(SELF_REF_ORDINAL, NO_ID);


	//reader/writer function specializations
	template <>
	MGFDataReader &MGFDataReader::operator >> <UID>(UID &uid)
	{
		*this >> uid.ordinal;

		if(mGameFile != nullptr)
		{
			uid.ordinal = mGameFile->localToGlobalOrdinal(uid.ordinal);

		}

		*this >> uid.id;

		return *this;
	}

	template <>
	MGFDataWriter &MGFDataWriter::operator << <UID>(const UID &uid)
	{
	    // TODO: No translation needed?

	    *this << uid.ordinal << uid.id;

	    return *this;
	}

	template <>
	MGFDataReader &MGFDataReader::operator >> <RecordHeader>(RecordHeader &header)
	{
		*this >> header.type >> header.dataSize;

		if(header.type != Record::TYPE_GROUP)
		{
			*this >> header.flags >> header.id;

		}else
		{
			*this >> header.flags >> header.groupType >> header.recordCount;
		}

		return *this;
	}

	template <>
    MGFDataWriter &MGFDataWriter::operator << <RecordHeader>(const RecordHeader &header)
    {
	    *this << header.type << header.dataSize;

        if(header.type != Record::TYPE_GROUP)
        {
            *this << header.flags << header.id;

        }else
        {
            *this << header.flags << header.groupType << header.recordCount;
        }

        return *this;
    }

	template <>
	MGFDataReader &MGFDataReader::operator >> <SubrecordHeader>(SubrecordHeader &header)
	{
		*this >> header.type >> header.dataSize;

		return *this;
	}

	template <>
	MGFDataWriter &MGFDataWriter::operator << <SubrecordHeader>(const SubrecordHeader &header)
	{
	    *this << header.type << header.dataSize;

	    return *this;
	}

}
