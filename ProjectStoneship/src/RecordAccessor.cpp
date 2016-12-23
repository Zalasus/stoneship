/*
 * RecordAccessor.cpp
 *
 *  Created on: Jan 8, 2016
 *      Author: zalasus
 */

#include "RecordAccessor.h"

#include "Record.h"
#include "MasterGameFile.h"
#include "Logger.h"

namespace Stoneship
{

    RecordAccessor::RecordAccessor(const RecordHeader &header, std::istream *stream, std::streampos dataOffset, MasterGameFile *mgf)
    : mHeader(header)
    , mStream(stream)
    , mGameFile(mgf)
    , mInternalReader(mStream, mgf, mHeader.dataSize)
    , mDataOffset(dataOffset)
    , mSubrecordHeaders()
    {
    }

    RecordAccessor::RecordAccessor(const RecordAccessor &a)
    : mHeader(a.mHeader)
    , mStream(a.mStream)
    , mGameFile(a.mGameFile)
    , mInternalReader(mStream, mGameFile, mHeader.dataSize)
    , mDataOffset(a.mDataOffset)
    , mSubrecordHeaders(a.mSubrecordHeaders)
    {
    }

    RecordAccessor::~RecordAccessor()
    {
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

    bool RecordAccessor::hasChildren() const
    {
        return (mHeader.type == Record::TYPE_GROUP) && (mHeader.recordCount > 0);
    }

    uint32_t RecordAccessor::getSubrecordCount()
    {
        if(mHeader.type == Record::TYPE_GROUP)
        {
            return 0;
        }

        if(mSubrecordHeaders.ptr() == nullptr)
        {
            rollback();

            _indexSubrecords();
        }

        return mSubrecordHeaders.size();
    }

    uint32_t RecordAccessor::getSubrecordCountForType(Record::Subtype subtype)
    {
        if(mSubrecordHeaders.ptr() == nullptr)
        {
            rollback();

            _indexSubrecords();
        }

        uint32_t count = 0;

        for(uint32_t i = 0; i <  mSubrecordHeaders.size(); ++i)
        {
            if(mSubrecordHeaders[i].type == subtype)
            {
                ++count;
            }
        }

        return count;
    }

    const SimpleArray<SubrecordHeader> &RecordAccessor::getSubrecordHeaders()
    {
        if(mSubrecordHeaders.ptr() == nullptr)
        {
            rollback();

            _indexSubrecords();
        }

        return mSubrecordHeaders;
    }

    MGFDataReader RecordAccessor::getReaderForSubrecord(Record::Subtype subtype)
    {
        // NOTE: Do not rollback here. Program relies on this method ignoring previous subrecords. rollback() has to be called manually

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

    MasterGameFile *RecordAccessor::getGameFile() const
    {
        return mGameFile;
    }

    std::streampos RecordAccessor::getDataOffset() const
    {
        return mDataOffset;
    }

    std::streampos RecordAccessor::getOffset() const
    {
        return static_cast<uint32_t>(mDataOffset) - mHeader.sizeInFile(); // cast is neccassary to make compiler stop complaining
    }

    RecordIterator RecordAccessor::toIterator() const
    {
        return RecordIterator(getOffset(), mStream, mGameFile);
    }

    RecordIterator RecordAccessor::getChildIterator() const
    {
        // no other record type than GROUP may contain child records; catch any invalid calls here
        if(mHeader.type != Record::TYPE_GROUP)
        {
            STONESHIP_EXCEPT(StoneshipException::INVALID_RECORD_TYPE, "Tried to access child records of non-GROUP record (UID: " + getUID().toString() + ", Type: " + mHeader.type + ")");
        }
        
        if(mHeader.recordCount == 0)
        {
            return getChildEnd();
        }
        
        return RecordIterator(mDataOffset, mStream, mGameFile);
    }
    
    RecordIterator RecordAccessor::getChildEnd() const
    {
        // no other record type than GROUP may contain child records; catch any invalid calls here
        if(mHeader.type != Record::TYPE_GROUP)
        {
            STONESHIP_EXCEPT(StoneshipException::INVALID_RECORD_TYPE, "Tried to access child records of non-GROUP record (UID: " + getUID().toString() + ", Type: " + mHeader.type + ")");
        }
        
        std::streampos pos = static_cast<uint32_t>(mDataOffset) + mHeader.dataSize;
        
        return RecordIterator(pos, mStream, mGameFile);
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
        Logger::info(String("Indexing subrecords of record ") + mHeader.id);

        //NOTE: No, I'm not gonna use a vector for that. I'm paying the bills here, so I get to choose the fucking container!

        //first, count how many subrecords we've got
        uint32_t subrecordCount = 0;
        while(mInternalReader.bytesRemainingInUnit())
        {
            SubrecordHeader header;
            mInternalReader >> header;

            ++subrecordCount;

            mInternalReader.skip(header.dataSize);
        }

        rollback();


        // next, read headers and store them

        mSubrecordHeaders.allocate(subrecordCount);

        for(uint32_t i = 0; i < mSubrecordHeaders.size(); ++i)
        {
            mInternalReader >> mSubrecordHeaders[i];

            mInternalReader.skip(mSubrecordHeaders[i].dataSize);
        }

        rollback(); //don't forget to rollback. indexing shouldn't be invasive
    }

}


