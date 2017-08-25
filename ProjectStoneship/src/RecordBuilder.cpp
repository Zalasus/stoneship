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

    RecordBuilder::RecordBuilder(MGFDataWriter &writer, RecordBuilder *parent)
    : mWriter(writer),
      mParent(parent),
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

        mRecordHeader.type = type;
        mRecordHeader.flags = flags;
        mRecordHeader.id = id;
        mRecordHeader.dataSize = 0xEFBEADDE; // this field is overwritten by endRecord(). if not, we will probably notice when viewing MGF in hex

        mRecordHeaderOffset = mWriter.tell();
        mWriter << mRecordHeader;

        // have we created an identifiable record here? set flag in all surrounding group if present
        if(mRecordHeader.id != UID::NO_ID)
        {
            RecordBuilder *b = mParent;

            while(b != nullptr && b->getType() == Record::TYPE_GROUP)
            {
                b->setFlags(b->getFlags() | RecordHeader::FLAG_ID_PRESENT);

                b = b->mParent;
            }
        }

        mBuildingRecord = true;
    }

    void RecordBuilder::beginGroupRecord(Record::Type groupType, RecordHeader::FlagType flags)
    {
        if(mBuildingSubrecord)
        {
            STONESHIP_EXCEPT(StoneshipException::INVALID_STATE, "Tried to begin building group record while subrecord was still beeing built.");
        }

        if(mBuildingRecord)
        {
            STONESHIP_EXCEPT(StoneshipException::INVALID_STATE, "Tried to begin building group record while other record was still beeing built.");
        }

        mRecordHeader.type = Record::TYPE_GROUP;
        mRecordHeader.groupType = groupType;
        mRecordHeader.recordCount = 0;
        mRecordHeader.dataSize = 0xEFBEADDE; // these fields are overwritten by endRecord(). if not, we will probably notice when viewing MGF in hex
        mRecordHeader.flags = flags;

        mRecordHeaderOffset = mWriter.tell();
        mWriter << mRecordHeader;
        mWrittenRecordHeader = mRecordHeader; // store what we've just written so we can check if we need to update it when it changed

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

        // store our current position. to finish the record, we may need to update it's header
        std::streamoff pos = mWriter.tell();

        // determine size of record data field (also subtracting header size)
        mRecordHeader.dataSize = pos - mRecordHeaderOffset - mRecordHeader.sizeInFile();

        // has the previously written header changed? if yes, update header in file
        if(mWrittenRecordHeader != mRecordHeader)
        {
            mWriter.seek(mRecordHeaderOffset);
            mWriter << mRecordHeader;

            mWriter.seek(pos);
        }

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

        if(mRecordHeader.type == Record::TYPE_GROUP)
        {
            STONESHIP_EXCEPT(StoneshipException::INVALID_STATE, "Tried to begin subrecord inside GROUP record.");
        }

        mSubrecordHeader.type = type;
        mSubrecordHeader.dataSize = dataSize; // predicted data size

        mSubrecordHeaderOffset = mWriter.tell();
        mWriter << mSubrecordHeader;
        mWrittenSubrecordHeader = mSubrecordHeader; // store what we've just written so we can check if we need to update it when it changed

        // is this an editor data subrecord? if yes, we set the appropriate flag in the encasing group and all groups surrounding that one
        if(mSubrecordHeader.type == Record::SUBTYPE_EDITOR_NAME)
        {
            RecordBuilder *b = mParent;

            while(b != nullptr && b->getType() == Record::TYPE_GROUP)
            {
                b->setFlags(b->getFlags() | RecordHeader::FLAG_EDATA_PRESENT);

                b = b->mParent;
            }
        }

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

        mSubrecordHeader.dataSize = pos - mSubrecordHeaderOffset - mSubrecordHeader.sizeInFile();

        if(mWrittenSubrecordHeader != mSubrecordHeader)
        {
            // subrecord header has changed. update it
            mWriter.seek(mSubrecordHeaderOffset);
            mWriter << mSubrecordHeader;

            mWriter.seek(pos);
        }

        mBuildingSubrecord = false;
    }

    uint32_t RecordBuilder::getChildRecordCount()
    {
        return mRecordHeader.recordCount;
    }

    RecordBuilder RecordBuilder::createChildBuilder()
    {
        if(!mBuildingRecord)
        {
            STONESHIP_EXCEPT(StoneshipException::INVALID_STATE, "Tried to begin child record while not building encasing GROUP record.");
        }

        if(mRecordHeader.type != Record::TYPE_GROUP)
        {
            STONESHIP_EXCEPT(StoneshipException::INVALID_RECORD_TYPE, "Tried to create child record in non-GROUP type record.");
        }

        RecordBuilder builder(mWriter, this);

        ++(mRecordHeader.recordCount);

        return builder;
    }

    RecordHeader::FlagType RecordBuilder::getFlags() const
    {
        return mRecordHeader.flags;
    }

    void RecordBuilder::setFlags(RecordHeader::FlagType flags)
    {
        mRecordHeader.flags = flags;
    }

    Record::Type RecordBuilder::getType() const
    {
        return mRecordHeader.type;
    }
}

