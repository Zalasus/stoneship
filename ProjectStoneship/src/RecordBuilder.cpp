/*
 * RecordBuilder.cpp
 *
 *  Created on: Jan 14, 2016
 *      Author: zalasus
 */

#include "RecordBuilder.h"

namespace Stoneship
{

    RecordBuilder::RecordBuilder(MGFDataWriter &writer, Record::Type type, RecordHeader::FlagType flags, UID::ID id, Record::Type groupType)
    : mWriter(writer),
	  mType(type),
	  mFlags(flags),
	  mID(id),
	  mGroupType(groupType),
      mChildRecordCount(0)
    {
    }

    void RecordBuilder::beginRecord()
    {
        mWriter.writeIntegral(mType);

        mRecordSizeFieldOffset = mWriter.tell();
        mWriter.writeIntegral<RecordHeader::SizeType>(0xDEADBEEF); // this field is overwritten by endRecord()

        if(mType == Record::TYPE_GROUP)
        {
            mWriter.writeIntegral(mGroupType);
            mChildRecordCountFieldOffset = mWriter.tell();
            mWriter.writeIntegral<RecordHeader::ChildRecordCountType>(0xDEADBEEF);  // this field is overwritten by endRecord()

        }else
        {
            mWriter.writeIntegral<RecordHeader::FlagType>(mFlags);
            mWriter.writeIntegral<UID::ID>(mID);
        }
    }

    void RecordBuilder::endRecord()
    {
        std::streamoff pos = mWriter.tell();

        RecordHeader::SizeType size = pos - mRecordSizeFieldOffset - sizeof(mRecordSizeFieldOffset) - sizeof(RecordHeader::FlagType) - sizeof(UID::ID);

        mWriter.seek(mRecordSizeFieldOffset);
        mWriter.writeIntegral<RecordHeader::SizeType>(size);

        if(mType == Record::TYPE_GROUP)
        {

        }else
        {

        }

        mWriter.seek(pos);
    }

    MGFDataWriter &RecordBuilder::beginSubrecord(Record::Subtype type)
    {

        mWriter.writeIntegral<Record::Subtype>(type);

        mSubrecordSizeFieldOffset = mWriter.tell();
        mWriter.writeIntegral<SubrecordHeader::SizeType>(0xDEADBEEF); // this is overwritten by endSubrecord()

        return mWriter;
    }

    void RecordBuilder::endSubrecord()
    {
        std::streamoff pos = mWriter.tell();

        SubrecordHeader::SizeType size = pos - mSubrecordSizeFieldOffset - sizeof(SubrecordHeader::SizeType);

        mWriter.seek(mSubrecordSizeFieldOffset);
        mWriter.writeIntegral<SubrecordHeader::SizeType>(size);

        mWriter.seek(pos);
    }

    RecordBuilder RecordBuilder::beginSubgroup(Record::Type groupType)
    {
        MGFDataWriter &writer = beginSubrecord(Record::SUBTYPE_SUBGROUP);

        return RecordBuilder(writer, Record::TYPE_GROUP, 0, UID::NO_ID, groupType);
    }

    void RecordBuilder::endSubgroup()
    {
        endSubrecord();
    }

    RecordBuilder RecordBuilder::createChildBuilder(Record::Type type)
    {
        if(mType != Record::TYPE_GROUP)
        {
            STONESHIP_EXCEPT(StoneshipException::INVALID_RECORD_TYPE, "Tried to create child record in non-GROUP type record.");
        }

        return RecordBuilder(mWriter, type);
    }
}

