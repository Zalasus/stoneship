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

        RecordBuilder(MGFDataWriter &writer, Record::Type type, RecordHeader::FlagType flags = 0, UID::ID id = UID::NO_ID, Record::Type groupType = Record::TYPE_RESERVED);

        void beginRecord();
        void endRecord();

        MGFDataWriter &beginSubrecord(Record::Subtype type);
        void endSubrecord();

        RecordBuilder beginSubgroup(Record::Type groupType);
        void endSubgroup();

        RecordBuilder createChildBuilder(Record::Type type);

        /**
         * Finalizes the current record, writes length fields and footers and initializes new
         * record header of given type after footer of current record. This object can now be used
         * to build new record.
         */
        void endRecordBeginNew(Record::Type type);

    private:

        MGFDataWriter &mWriter;
        Record::Type mType;
        RecordHeader::FlagType mFlags;
        UID::ID mID;
        Record::Type mGroupType;

        std::streampos mRecordSizeFieldOffset;
        std::streampos mChildRecordCountFieldOffset; // for GROUP records only
        std::streampos mSubrecordSizeFieldOffset;

        RecordHeader::ChildRecordCountType mChildRecordCount;

    };

}



#endif /* INCLUDE_RECORDBUILDER_H_ */
