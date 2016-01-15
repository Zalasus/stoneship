/*
 * IRecordStoreable.h
 *
 *  Created on: Jan 5, 2016
 *      Author: zalasus
 */

#ifndef INCLUDE_IRECORDSTOREABLE_H_
#define INCLUDE_IRECORDSTOREABLE_H_

namespace Stoneship
{

    class RecordBuilder;

    class IRecordStoreable
    {
    public:

        IRecordStoreable();

        virtual ~IRecordStoreable();

        virtual void storeToRecord(RecordBuilder &rec) = 0;

        /**
         * A Storable is dirty right after it was created, and should be until it is considered to be in
         * a valid state and and recreatable in exactly that state (e.g. right after it was loaded from a record/
         * it was written to a record)
         */
        virtual bool isDirty();
        virtual void setDirty(bool b);

    private:

        bool mRecordDirty;
    };

}



#endif /* INCLUDE_IRECORDSTOREABLE_H_ */
