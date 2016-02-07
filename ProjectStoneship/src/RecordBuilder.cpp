/*
 * RecordBuilder.cpp
 *
 *  Created on: Jan 14, 2016
 *      Author: zalasus
 */

#include "RecordBuilder.h"

namespace Stoneship
{

    RecordBuilder::RecordBuilder(MGFDataWriter &writer)
    : mWriter(writer),
	  mType(Record::TYPE_RESERVED),
	  mFlags(0),
	  mID(UID::NO_ID),
	  mGroupType(Record::TYPE_RESERVED),
      mChildRecordCount(0)
    {
    }

    void RecordBuilder::beginRecord(Record::Type type, RecordHeader::FlagType flags, UID::ID id)
    {
        if(type == Record::TYPE_GROUP)
        {
            STONESHIP_EXCEPT(StoneshipException::INVALID_RECORD_TYPE, "Called beginRecord() in RecordBuilder for GROUP record. Must use beginGroupRecord() instead. This indicated a bug in the engine.");
        }

        mType = type;
        mFlags = flags;
        mID = id;


        mWriter << mType;

        mRecordSizeFieldOffset = mWriter.tell();
        mWriter << static_cast<RecordHeader::SizeType>(0xDEADBEEF) // this field is overwritten by endRecord()
                << mFlags
                << mID;
    }

    void RecordBuilder::beginGroupRecord(Record::Type groupType)
    {
        mType = Record::TYPE_GROUP;
        mGroupType = groupType;

        mWriter << mType;

        mRecordSizeFieldOffset = mWriter.tell();
        mWriter << static_cast<RecordHeader::SizeType>(0xDEADBEEF); // this field is overwritten by endRecord()

        mWriter << mGroupType;
        mChildRecordCountFieldOffset = mWriter.tell();
        mWriter << static_cast<RecordHeader::ChildRecordCountType>(0xDEADBEEF);  // this field is overwritten by endRecord()
    }

    void RecordBuilder::endRecord()
    {
        // store our current position. to finish the record, we need to update it's header
        std::streamoff pos = mWriter.tell();

        // determine size of record data field, subtracting remaining header fields
        RecordHeader::SizeType size = pos - mRecordSizeFieldOffset - sizeof(RecordHeader::SizeType) - sizeof(RecordHeader::FlagType) - sizeof(UID::ID);

        mWriter.seek(mRecordSizeFieldOffset);
        mWriter << size;

        if(mType == Record::TYPE_GROUP)
        {
            // in a group record, we have to overwrite the child record count field

            mWriter.seek(mChildRecordCountFieldOffset);
            mWriter << mChildRecordCount;
        }

        // we are done. return to record footer
        mWriter.seek(pos);
    }

    MGFDataWriter &RecordBuilder::beginSubrecord(Record::Subtype type)
    {

        mWriter << type;

        mSubrecordSizeFieldOffset = mWriter.tell();
        mWriter << static_cast<SubrecordHeader::SizeType>(0xDEADBEEF); // this is overwritten by endSubrecord()

        return mWriter;
    }

    RecordBuilder RecordBuilder::beginSubgroupSubrecord(Record::Type groupType)
    {
        MGFDataWriter &writer = beginSubrecord(Record::SUBTYPE_SUBGROUP);

        RecordBuilder builder(writer);
        builder.beginGroupRecord(groupType);

        return builder;
    }

    void RecordBuilder::endSubrecord()
    {
        std::streamoff pos = mWriter.tell();

        SubrecordHeader::SizeType size = pos - mSubrecordSizeFieldOffset - sizeof(SubrecordHeader::SizeType);

        mWriter.seek(mSubrecordSizeFieldOffset);
        mWriter << size;

        mWriter.seek(pos);
    }

    RecordBuilder RecordBuilder::createAndBeginChildBuilder(Record::Type type, RecordHeader::FlagType flags, UID::ID id)
    {
        if(mType != Record::TYPE_GROUP)
        {
            STONESHIP_EXCEPT(StoneshipException::INVALID_RECORD_TYPE, "Tried to create child record in non-GROUP type record.");
        }

        RecordBuilder builder(mWriter);
        builder.beginRecord(type, flags, id);

        ++mChildRecordCount;

        return builder;
    }
}

