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

        RecordBuilder(Record::Type type, MGFDataWriter &writer);

        void beginRecord(RecordHeader::FlagType flags, UID::ID id, Record::Type groupType);
        void endRecord();

        MGFDataWriter &beginSubrecord(Record::Subtype type);
        void endSubrecord();

        RecordBuilder beginSubgroup();
        void endSubgroup();

        RecordBuilder createChildBuilder(Record::Type type);

        /**
         * Finalizes the current record, writes length fields and footers and initializes new
         * record header of given type after footer of current record. This object can now be used
         * to build new record.
         */
        void endRecordBeginNew(Record::Type type);

    private:

        Record::Type mType;
        MGFDataWriter &mWriter;

        std::streampos mRecordSizeFieldOffset;
        std::streampos mChildRecordCountFieldOffset; // for GROUP records only
        std::streampos mSubrecordSizeFieldOffset;

        RecordHeader::ChildRecordCountType mChildRecordCount;

    };

}



#endif /* INCLUDE_RECORDBUILDER_H_ */
