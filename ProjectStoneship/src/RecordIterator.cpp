#include "RecordIterator.h"

#include "MasterGameFile.h"
#include "RecordAccessor.h"

namespace Stoneship
{
    
    RecordIterator::RecordIterator(std::streampos offset, std::istream *stream, MasterGameFile *mgf)
    : mOffset(offset)
    , mStream(stream)
    , mGameFile(mgf)
    {
    }

    RecordIterator::~RecordIterator()
    {
    }

    RecordAccessor &RecordIterator::operator*()
    {
        return getAccessorRef();
    }
    
    RecordAccessor *RecordIterator::operator->()
    {
        return &getAccessorRef();
    }
    
    RecordIterator &RecordIterator::operator++()
    {
        next();
        
        return *this;
    }
    
    RecordIterator RecordIterator::operator++(int)
    {
        RecordIterator it(*this);
        
        next();
        
        return it; 
    }
    
    RecordIterator &RecordIterator::next()
    {
        mOffset = static_cast<size_t>(mOffset) + getAccessorRef().getHeader().dataSize + getAccessorRef().getHeader().sizeInFile();
        
        return *this;
    }
    
    RecordAccessor RecordIterator::getAccessor()
    {
        return getAccessorRef(); // return copy that can be safely handled
    }
            
    RecordAccessor &RecordIterator::getAccessorRef()
    {
        if(mStream == nullptr)
        {
            STONESHIP_EXCEPT(StoneshipException::IO_ERROR, "Tried to read record at invalid position");
        }
        
        static RecordAccessor cachedAccessor(RecordHeader(), mStream, 0, nullptr);
        static RecordIterator *lastUser = this;
        
        if(cachedAccessor.getGameFile() == nullptr || cachedAccessor.getOffset() != mOffset || lastUser != this)
        {
            // iterator has moved, accessor was not yet initialized properly or different iterator is using cache. create new accessor
            
            mStream->seekg(mOffset);
            MGFDataReader ds(mStream, mGameFile);

            RecordHeader header;
            ds >> header;
            
            cachedAccessor = RecordAccessor(header, mStream, static_cast<size_t>(mOffset) + header.sizeInFile(), mGameFile);
        }
        
        return cachedAccessor;
    }
    
    std::streampos RecordIterator::getOffset() const
    {
        return mOffset;
    }
    
    bool operator==(const RecordIterator &a, const RecordIterator &b)
    {
        return a.getOffset() == b.getOffset();
    }
    
    bool operator!=(const RecordIterator &a, const RecordIterator &b)
    {
        return a.getOffset() != b.getOffset();
    }
    
    bool operator>=(const RecordIterator &a, const RecordIterator &b)
    {
        return a.getOffset() >= b.getOffset();
    }
}
