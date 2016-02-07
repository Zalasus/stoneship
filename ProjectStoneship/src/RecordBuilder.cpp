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
        mWriter << mType;

        mRecordSizeFieldOffset = mWriter.tell();
        mWriter << static_cast<RecordHeader::SizeType>(0xDEADBEEF); // this field is overwritten by endRecord()

        if(mType == Record::TYPE_GROUP)
        {
            mWriter << mGroupType;
            mChildRecordCountFieldOffset = mWriter.tell();
            mWriter << static_cast<RecordHeader::ChildRecordCountType>(0xDEADBEEF);  // this field is overwritten by endRecord()

        }else
        {
            mWriter << mFlags;
            mWriter << mID;
        }
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

    void RecordBuilder::endSubrecord()
    {
        std::streamoff pos = mWriter.tell();

        SubrecordHeader::SizeType size = pos - mSubrecordSizeFieldOffset - sizeof(SubrecordHeader::SizeType);

        mWriter.seek(mSubrecordSizeFieldOffset);
        mWriter << size;

        mWriter.seek(pos);
    }

    RecordBuilder RecordBuilder::beginSubgroup(Record::Type groupType)
    {
        MGFDataWriter &writer = beginSubrecord(Record::SUBTYPE_SUBGROUP);

        RecordBuilder builder(writer, Record::TYPE_GROUP, 0, UID::NO_ID, groupType);
        builder.beginRecord();

        return builder;
    }

    void RecordBuilder::endSubgroup()
    {
        endSubrecord();
    }

    RecordBuilder RecordBuilder::createChildBuilder(Record::Type type, RecordHeader::FlagType flags, UID::ID id)
    {
        if(mType != Record::TYPE_GROUP)
        {
            STONESHIP_EXCEPT(StoneshipException::INVALID_RECORD_TYPE, "Tried to create child record in non-GROUP type record.");
        }

        RecordBuilder builder(mWriter, type, flags, id);

        // since child records are entirely up to the entity creating it, we don't write headers for them
        //builder.beginRecord();

        ++mChildRecordCount;

        return builder;
    }

    void RecordBuilder::endRecordBeginNew(Record::Type type, RecordHeader::FlagType flags, UID::ID id, Record::Type groupType)
    {
        endRecord();

        mType = type;
        mFlags = flags;
        mID = id;
        mGroupType = groupType;

        beginRecord();
    }
}

