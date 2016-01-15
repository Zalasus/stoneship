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

namespace Stoneship
{

    class MasterGameFile;

    class RecordAccessor
    {
    public:

        RecordAccessor(const RecordHeader &header, std::istream *stream, MasterGameFile *mgf);
        RecordAccessor(const RecordAccessor &a);
        ~RecordAccessor();

        const RecordHeader &getHeader() const;
        MasterGameFile *getGameFile();
        std::streampos getDataOffset(); /** @returns Offset of record's data field */
        std::streampos getOffset(); /** @returns Absolute offset of record */

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
         * @brief Creates Accessor for the next record following this one in the stream.
         *
         * Once this method was called, the accessor gets unusable for subrecord access unless rollback() is called. This method can be called
         * in any stream position as long as it isn't EOF.
         */
        RecordAccessor getNextRecord();

        /**
         * @brief Creates Accessor for the first record in this record's data field.
         *
         * This method is insensitive to stream position and may be called everywhere in the stream. In accordance to the MGF specification,
         * this method may only be called on RecordAccessors for GROUP records. Any other type will cause this method to throw.
         */
        RecordAccessor getFirstChildRecord();

        /**
         * @brief Creates Accessor for GROUP record in subrecord of _SUBGROUP type.
         *
         * This method will throw if no _SUBGROUP subrecord is found.
         */
        RecordAccessor getSubgroup();

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
        uint32_t mSubrecordCount;
        SubrecordHeader *mSubrecordHeaders;

    };

}



#endif /* INCLUDE_RECORDACCESSOR_H_ */
