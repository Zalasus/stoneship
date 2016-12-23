#ifndef RECORDITERATOR_H
#define RECORDITERATOR_H

#include <istream>

namespace Stoneship
{
    
    class MasterGameFile;
    class RecordAccessor;
    
    class RecordIterator 
    {
        public:
            RecordIterator(std::streampos offset, std::istream *stream, MasterGameFile *mgf);
            virtual ~RecordIterator();
            
            RecordAccessor &operator*();
            RecordAccessor *operator->();
            RecordIterator &operator++();
            RecordIterator operator++(int);
            
            RecordIterator &next();
            
            RecordAccessor getAccessor();  // THIS MUST NOT RETURN REFERENCE!!! Stupid shit happened because some methods were using the same accessor by accident.
            RecordAccessor &getAccessorRef();
            std::streampos getOffset() const;

        protected:

        private:
            
            std::streampos mOffset;
            std::istream *mStream;
            MasterGameFile *mGameFile;
    };
    
    bool operator==(const RecordIterator &a, const RecordIterator &b);
    bool operator!=(const RecordIterator &a, const RecordIterator &b);
    bool operator>=(const RecordIterator &a, const RecordIterator &b);
}

#endif // RECORDITERATOR_H
