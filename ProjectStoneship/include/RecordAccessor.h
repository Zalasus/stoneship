/*
 * RecordAccessor.h
 *
 *  Created on: Jan 8, 2016
 *      Author: zalasus
 */

#ifndef INCLUDE_RECORDACCESSOR_H_
#define INCLUDE_RECORDACCESSOR_H_

#include "Record.h"
#include "MGFDataReader.h"
#include "cont/SimpleArray.h"
#include "RecordIterator.h"

namespace Stoneship
{

    class MasterGameFile;

    class RecordAccessor
    {
    public:

        RecordAccessor(const RecordHeader &header, std::istream *stream, std::streampos dataOffset, MasterGameFile *mgf);
        RecordAccessor(const RecordAccessor &a);
        ~RecordAccessor();

        const RecordHeader &getHeader() const;
        MasterGameFile *getGameFile() const;
        std::streampos getDataOffset() const; /** @returns Offset of record's data field */
        std::streampos getOffset() const; /** @returns Absolute offset of record */
        
        /**
         * @returns true if record type is GROUP and child count is > 0
         */
        bool hasChildren() const;

        UID getUID() const;

        MGFDataReader &getReader();

        /**
         * @brief Retrieves the amount of subrecords this record stores.
         *
         * This method returns the amount of subrecords contained in this record's data field. It operates on
         * the internal index of subrecords, which is not aquired by default. If this method is called on an
         * Accessor with empty index, it is filled before the call returns. Subsequent calls will not require
         * another indexing.
         */
        uint32_t getSubrecordCount();

        /**
         * @brief Retrieves the amount of subrecords this record stores that match the given type.
         *
         * This method returns the amount of subrecords contained in this record's data field which match the
         * type given by parameter subtype. It operates on the internal index of subrecords, which is not
         * aquired by default. If this method is called on an Accessor with empty index, it is filled before
         * the call returns. Subsequent calls will not require another indexing.
         */
        uint32_t getSubrecordCountForType(Record::Subtype subtype);

        /**
         * This method returns an array of all subrecord headers contained in this record in the order they appear
         * in the MGF. It operates on the internal index of subrecords, which is not
         * aquired by default. If this method is called on an Accessor with empty index, it is filled before
         * the call returns. Subsequent calls will not require another indexing.
         */
        const SimpleArray<SubrecordHeader> &getSubrecordHeaders();

        /**
         * @brief Creates data reader for first subrecord of given type to be found.
         *
         * Searches this record's data field for the first subrecord of the given type,
         * creates and returns a data reader for it. The returned reader will be locked to
         * the found subrecord's size, so it can't read outside the bounds of the subrecord.
         *
         * If the given subrecord type is not found in the record, this method will throw.
         *
         * Since according to the MGF spec a GROUP record may not contain subrecords, this
         * method will also throw if called for such a record.
         *
         * TODO: Facilities to handle multiple subrecords of the same type in a single record have yet to be implemented if needed.
         */
        MGFDataReader getReaderForSubrecord(Record::Subtype subtype);

        /**
         * @brief Creates Iterator at beginning of this record.
         */
        RecordIterator toIterator() const;

        
        /**
         * @brief Creates Iterator for the first record in this record's data field.
         *
         * If this record does not contain any child records, getChildIterator() will return an Iterator pointing to
         * the record's end instead (same ad getChildEnd()).
         *
         * This method is insensitive to stream position and may be called everywhere in the stream. In accordance to the MGF specification,
         * this method may only be called on RecordAccessors for GROUP records. Any other type will cause this method to throw.
         */
        RecordIterator getChildIterator() const;
        
        /**
         * @brief Creates Iterator for next record in series (right at the end of current record).
         */
        RecordIterator getChildEnd() const;
        
        /**
         * @brief Moves the stream pointer to the beginning of this records data field.
         */
        void rollback();

        /**
         * @brief Moves the stream pointer to the end of this record (the first byte after the data field)
         */
        void skip();

    private:

        void _indexSubrecords();

        RecordHeader mHeader;
        std::istream *mStream;
        MasterGameFile *mGameFile;
        MGFDataReader mInternalReader;
        std::streampos mDataOffset; //offset of data field
        SimpleArray<SubrecordHeader> mSubrecordHeaders;

    };

}



#endif /* INCLUDE_RECORDACCESSOR_H_ */
