/*
 * RecordBuilder.h
 *
 *  Created on: Jan 5, 2016
 *      Author: zalasus
 */

#ifndef INCLUDE_RECORDBUILDER_H_
#define INCLUDE_RECORDBUILDER_H_

#include "Record.h"
#include "MGFDataWriter.h"

namespace Stoneship
{

    class RecordBuilder
    {
    public:

        RecordBuilder(MGFDataWriter &writer);

        void beginRecord(Record::Type type, RecordHeader::FlagType flags, UID::ID id);
        void beginGroupRecord(Record::Type groupType);
        void endRecord();

        MGFDataWriter &beginSubrecord(Record::Subtype type);
        /**
         * Starts a subrecord of given type and initializes header with given data size. This saves the effort of having to seek back to
         * the record header to fill in the correct size field after calling endSubrecord(). If this method is used to begin a subrecord,
         * and the subsequent call to endSubrecord() finds that more bytes have been written than passed to this method, the size field is
         * overwritten anyway.
         */
        MGFDataWriter &beginSubrecord(Record::Subtype type, SubrecordHeader::SizeType dataSize);

        /**
         * Starts a subrecord of _SUBGROUP type and creates and initializes a Builder for contained GROUP record. Headers are
         * written by this method, so the caller can use the returned Builder for IO operations right away. It is, however, neccessary
         * for the caller to call endRecord() on the returned Builder. This call would usually be followed by a call to endSubrecord()
         */
        RecordBuilder beginSubgroupSubrecord(Record::Type groupType);

        void endSubrecord();

        /**
         * Returns the amount of child records that were written to this record so far. In accordance with the MGF spec, only GROUP
         * records may contain child records, so this method will return invalid data if called on a non-GROUP type record.
         */
        uint32_t getChildRecordCount();

        /**
         * Creates a Builder for a new child record and writes headers for the new child. The returned builder can be used for IO right away,
         * as there is no need for the caller to invoke beginRecord anymore. In accordance with the MGF spec, only GROUP
         * records may contain child records, so this method will throw if called on a non-GROUP type record.
         */
        RecordBuilder createAndBeginChildBuilder(Record::Type type, RecordHeader::FlagType flags, UID::ID id);

    private:

        MGFDataWriter &mWriter;
        Record::Type mType;
        RecordHeader::FlagType mFlags;
        UID::ID mID;
        Record::Type mGroupType;
        Record::Subtype mSubrecordType;
        SubrecordHeader::SizeType mPredictedDataSize;

        std::streampos mRecordSizeFieldOffset;
        std::streampos mChildRecordCountFieldOffset; // for GROUP records only
        std::streampos mSubrecordSizeFieldOffset;

        RecordHeader::ChildRecordCountType mChildRecordCount;

        bool mBuildingSubrecord; // true if subrecord is beeing build ATM
        bool mBuildingRecord; // true if record is beeing build ATM

    };

}



#endif /* INCLUDE_RECORDBUILDER_H_ */
