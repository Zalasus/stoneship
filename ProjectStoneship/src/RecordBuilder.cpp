/*
 * RecordBuilder.cpp
 *
 *  Created on: Jan 14, 2016
 *      Author: zalasus
 */

#include "RecordBuilder.h"

#include "Logger.h"

namespace Stoneship
{

    RecordBuilder::RecordBuilder(MGFDataWriter &writer)
    : mWriter(writer),
	  mType(Record::TYPE_RESERVED),
	  mFlags(0),
	  mID(UID::NO_ID),
	  mGroupType(Record::TYPE_RESERVED),
	  mSubrecordType(0),
      mPredictedDataSize(0),
      mChildRecordCount(0),
      mBuildingSubrecord(false),
      mBuildingRecord(false)
    {

    }

    void RecordBuilder::beginRecord(Record::Type type, RecordHeader::FlagType flags, UID::ID id)
    {
        if(mBuildingSubrecord)
        {
            STONESHIP_EXCEPT(StoneshipException::INVALID_STATE, "Tried to begin building record while subrecord was still beeing built.");
        }

        if(mBuildingRecord)
        {
            STONESHIP_EXCEPT(StoneshipException::INVALID_STATE, "Tried to begin building record while other record was still beeing built.");
        }

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

        mBuildingRecord = true;
    }

    void RecordBuilder::beginGroupRecord(Record::Type groupType)
    {
        if(mBuildingSubrecord)
        {
            STONESHIP_EXCEPT(StoneshipException::INVALID_STATE, "Tried to begin building group record while subrecord was still beeing built.");
        }

        if(mBuildingRecord)
        {
            STONESHIP_EXCEPT(StoneshipException::INVALID_STATE, "Tried to begin building group record while other record was still beeing built.");
        }

        mType = Record::TYPE_GROUP;
        mGroupType = groupType;
        mChildRecordCount = 0;

        mWriter << mType;

        mRecordSizeFieldOffset = mWriter.tell();
        mWriter << static_cast<RecordHeader::SizeType>(0xDEADBEEF); // this field is overwritten by endRecord()

        mWriter << mGroupType;
        mChildRecordCountFieldOffset = mWriter.tell();
        mWriter << static_cast<RecordHeader::ChildRecordCountType>(0xDEADBEEF);  // this field is overwritten by endRecord()

        mBuildingRecord = true;
    }

    void RecordBuilder::endRecord()
    {
        if(mBuildingSubrecord)
        {
            STONESHIP_EXCEPT(StoneshipException::INVALID_STATE, "Finished record while still building subrecord. Did you call endRecord() instead of endSubrecord()?");
        }

        if(!mBuildingRecord)
        {
            STONESHIP_EXCEPT(StoneshipException::INVALID_STATE, "Tried to end record while no record was beeing built.");
        }

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

        mBuildingRecord = false;
    }

    MGFDataWriter &RecordBuilder::beginSubrecord(Record::Subtype type)
    {
        return beginSubrecord(type, 0); // we just make the static prediction that no bytes will be written TODO: might want to check other values to archive minimum throughput in typical application
    }

    MGFDataWriter &RecordBuilder::beginSubrecord(Record::Subtype type, SubrecordHeader::SizeType dataSize)
    {
        if(mBuildingSubrecord)
        {
            STONESHIP_EXCEPT(StoneshipException::INVALID_STATE, "Tried to begin new subrecord while still building other one.");
        }

        if(!mBuildingRecord)
        {
            STONESHIP_EXCEPT(StoneshipException::INVALID_STATE, "Tried to begin subrecord while not building encasing record.");
        }

        if(mType == Record::TYPE_GROUP)
        {
            STONESHIP_EXCEPT(StoneshipException::INVALID_STATE, "Tried to begin subrecord inside GROUP record.");
        }

        mPredictedDataSize = dataSize;
        mSubrecordType = type;

        mWriter << type;

        mSubrecordSizeFieldOffset = mWriter.tell();
        mWriter << mPredictedDataSize; // this is overwritten by endSubrecord()

        mBuildingSubrecord = true;

        return mWriter;
    }

    /*RecordBuilder RecordBuilder::beginSubgroupSubrecord(Record::Type groupType)
    {
        MGFDataWriter &writer = beginSubrecord(Record::SUBTYPE_SUBGROUP);

        RecordBuilder builder(writer);
        builder.beginGroupRecord(groupType);

        return builder;
    }*/

    void RecordBuilder::endSubrecord()
    {
        if(!mBuildingSubrecord)
        {
            STONESHIP_EXCEPT(StoneshipException::INVALID_STATE, "Tried to end subrecord when no subrecord was beeing built.");
        }

        std::streamoff pos = mWriter.tell();

        SubrecordHeader::SizeType size = pos - mSubrecordSizeFieldOffset - sizeof(SubrecordHeader::SizeType);

        if(size != mPredictedDataSize)
        {
            // we have written the wrong data size before. overwrite it
            mWriter.seek(mSubrecordSizeFieldOffset);
            mWriter << size;
            mWriter.seek(pos);
        }

        mBuildingSubrecord = false;
    }

    uint32_t RecordBuilder::getChildRecordCount()
    {
        return mChildRecordCount;
    }

    RecordBuilder RecordBuilder::createAndBeginChildBuilder(Record::Type type, RecordHeader::FlagType flags, UID::ID id)
    {
        if(!mBuildingRecord)
        {
            STONESHIP_EXCEPT(StoneshipException::INVALID_STATE, "Tried to begin child record while not building encasing GROUP record.");
        }

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

