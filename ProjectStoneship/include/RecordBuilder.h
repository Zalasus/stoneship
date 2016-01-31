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

        /**
         * Starts a subrecord of _SUBGROUP type and creates and initializes a Builder for contained GROUP record. Headers are
         * written by this method, so the caller can use the returned Builder for IO operations right away. It is, however, neccessary
         * for the caller to call endRecord() on the returned Builder. This call would usually be followed by a call to endSubgroup()
         */
        RecordBuilder beginSubgroup(Record::Type groupType);
        void endSubgroup();

        /**
         * Creates a Builder for new child record. This method merely creates the Builder and does not write any
         * record headers, so the caller has to invoke beginRecord() on the returned Builder before any IO operations.
         * In accordance with the MGF spec, only GROUP records may contain child records, so this method will throw if
         * called on a non-GROUP type record.
         */
        RecordBuilder createChildBuilder(Record::Type type, RecordHeader::FlagType flags, UID::ID id);

        /**
         * Finalizes the current record, writes length fields and footers and initializes new
         * record header of given type after footer of current record. This object can now be used
         * to build new record.
         */
        void endRecordBeginNew(Record::Type type, RecordHeader::FlagType flags = 0, UID::ID id = UID::NO_ID, Record::Type groupType = Record::TYPE_RESERVED);

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
