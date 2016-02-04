/*
 * RecordAccessor.cpp
 *
 *  Created on: Jan 8, 2016
 *      Author: zalasus
 */

#include "RecordAccessor.h"

#include "Record.h"
#include "MasterGameFile.h"

namespace Stoneship
{

    RecordAccessor::RecordAccessor(const RecordHeader &header, std::istream *stream, MasterGameFile *mgf)
    : mHeader(header),
      mStream(stream),
      mGameFile(mgf),
      mInternalReader(mStream, mgf, mHeader.dataSize),
      mDataOffset(mStream->tellg()),
      mSubrecordCount(0),
      mSubrecordHeaders(nullptr)
    {
    }

    RecordAccessor::RecordAccessor(const RecordAccessor &a)
    : mHeader(a.mHeader),
      mStream(a.mStream),
      mGameFile(a.mGameFile),
      mInternalReader(mStream, mGameFile, mHeader.dataSize),
      mDataOffset(a.mDataOffset),
      mSubrecordCount(a.mSubrecordCount),
      mSubrecordHeaders(nullptr)
    {
        if(a.mSubrecordHeaders != nullptr)
        {
            //create deep copy of index field
            mSubrecordHeaders = new SubrecordHeader[mSubrecordCount];

            for(uint32_t i = 0; i < mSubrecordCount; ++i)
            {
                mSubrecordHeaders[i] = a.mSubrecordHeaders[i];
            }
        }

    }

    RecordAccessor::~RecordAccessor()
    {
        if(mSubrecordHeaders != nullptr)
        {
            delete[] mSubrecordHeaders;

            mSubrecordHeaders = nullptr;
        }
    }

    const RecordHeader &RecordAccessor::getHeader() const
    {
        return mHeader;
    }

    UID RecordAccessor::getUID() const
    {
        return UID(mGameFile->getOrdinal(), mHeader.id);
    }

    MGFDataReader &RecordAccessor::getReader()
    {
        return mInternalReader;
    }

    uint32_t RecordAccessor::getSubrecordCount()
    {
        if(mHeader.type == Record::TYPE_GROUP)
        {
            return 0;
        }

        if(mSubrecordHeaders == nullptr)
        {
            rollback();

            _indexSubrecords();
        }

        return mSubrecordCount;
    }

    uint32_t RecordAccessor::getSubrecordCountForType(Record::Subtype subtype)
    {
        if(mSubrecordHeaders == nullptr)
        {
            rollback();

            _indexSubrecords();
        }

        uint32_t count = 0;

        for(uint32_t i = 0; i <  mSubrecordCount; ++i)
        {
            if(mSubrecordHeaders[i].type == subtype)
            {
                ++count;
            }
        }

        return count;
    }

    MGFDataReader RecordAccessor::getReaderForSubrecord(Record::Subtype subtype)
    {
        // a GROUP does never contain subrecords, so catch any invalid calls here
        if(mHeader.type == Record::TYPE_GROUP)
        {
            STONESHIP_EXCEPT(StoneshipException::INVALID_RECORD_TYPE, "Tried to access subrecords of GROUP record (UID: " + getUID().toString() + ", Type: " + mHeader.type + ")");
        }

        while(mInternalReader.bytesRemainingInUnit())
        {
            SubrecordHeader header;
            mInternalReader >> header;

            if(header.type == subtype) //found fitting subrecord
            {
                return MGFDataReader(mStream, mGameFile, header.dataSize);

            }else
            {
                mInternalReader.skip(header.dataSize);
            }
        }

        STONESHIP_EXCEPT(StoneshipException::SUBRECORD_NOT_FOUND, String("Subrecord ") + subtype + " not found in record (UID: " + getUID().toString() + ", Type: " + mHeader.type + ")");
    }

    MasterGameFile *RecordAccessor::getGameFile()
    {
        return mGameFile;
    }

    std::streampos RecordAccessor::getDataOffset()
    {
        return mDataOffset;
    }

    std::streampos RecordAccessor::getOffset()
    {
        return static_cast<uint32_t>(mDataOffset) - RecordHeader::SIZE_IN_FILE; // cast is neccassary to make compiler stop complaining
    }

    RecordAccessor RecordAccessor::getNextRecord()
    {
        mStream->seekg(static_cast<uint32_t>(mDataOffset) + mHeader.dataSize);
        MGFDataReader ds(mStream, mGameFile);

        RecordHeader header;
        ds >> header;

        return RecordAccessor(header, mStream, mGameFile);
    }

    RecordAccessor RecordAccessor::getFirstChildRecord()
    {
        // no other record type than GROUP may contain child records; catch any invalid calls here
        if(mHeader.type != Record::TYPE_GROUP)
        {
            STONESHIP_EXCEPT(StoneshipException::INVALID_RECORD_TYPE, "Tried to access child records of non-GROUP record (UID: " + getUID().toString() + ", Type: " + mHeader.type + ")");
        }

        rollback();

        RecordHeader header;
        mInternalReader >> header;

        return RecordAccessor(header, mStream, mGameFile);
    }

    RecordAccessor RecordAccessor::getSubgroup()
    {
        // a GROUP does never contain subrecords, so catch any invalid calls here
        if(mHeader.type == Record::TYPE_GROUP)
        {
            STONESHIP_EXCEPT(StoneshipException::INVALID_RECORD_TYPE, "Tried to access subrecords of GROUP record (UID: " + getUID().toString() + ", Type: " + mHeader.type + ")");
        }

        RecordHeader header;
        getReaderForSubrecord(Record::SUBTYPE_SUBGROUP) >> header;

        if(header.type != Record::TYPE_GROUP)
        {
            STONESHIP_EXCEPT(StoneshipException::INVALID_RECORD_TYPE, "No GROUP record found in _SUBGROUP subrecord (UID: " + getUID().toString() + ", Type: " + mHeader.type + ")");
        }

        return RecordAccessor(header, mInternalReader.getStream(), mGameFile);
    }

    void RecordAccessor::rollback()
    {
        mInternalReader.seek(mDataOffset);
    }

    void RecordAccessor::skip()
    {
        mStream->seekg(static_cast<uint32_t>(mDataOffset) + mHeader.dataSize);
    }

    void RecordAccessor::_indexSubrecords()
    {
        //NOTE: No, I'm not gonna use a vector for that. I'm paying the bills here, so I get to choose the fucking container!

        //first, count how many subrecords we've got
        while(mInternalReader.bytesRemainingInUnit())
        {
            SubrecordHeader header;
            mInternalReader >> header;

            ++mSubrecordCount;

            mInternalReader.skip(header.dataSize);
        }

        rollback();


        // next, read headers and store them

        mSubrecordHeaders = new SubrecordHeader[mSubrecordCount];

        for(uint32_t i = 0; i < mSubrecordCount; ++i)
        {
            mInternalReader >> mSubrecordHeaders[i];

            mInternalReader.skip(mSubrecordHeaders[i].dataSize);
        }

        rollback(); //don't forget to rollback. indexing shouldn't be invasive
    }

}


