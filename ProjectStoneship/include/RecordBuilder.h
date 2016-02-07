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
         * Starts a subrecord of _SUBGROUP type and creates and initializes a Builder for contained GROUP record. Headers are
         * written by this method, so the caller can use the returned Builder for IO operations right away. It is, however, neccessary
         * for the caller to call endRecord() on the returned Builder. This call would usually be followed by a call to endSubrecord()
         */
        RecordBuilder beginSubgroupSubrecord(Record::Type groupType);

        void endSubrecord();

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

        std::streampos mRecordSizeFieldOffset;
        std::streampos mChildRecordCountFieldOffset; // for GROUP records only
        std::streampos mSubrecordSizeFieldOffset;

        RecordHeader::ChildRecordCountType mChildRecordCount;

    };

}



#endif /* INCLUDE_RECORDBUILDER_H_ */
