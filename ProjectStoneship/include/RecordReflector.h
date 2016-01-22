/*
 * IRecordReflector.h
 *
 *  Created on: Jan 20, 2016
 *      Author: zalasus
 */

#ifndef INCLUDE_RECORDREFLECTOR_H_
#define INCLUDE_RECORDREFLECTOR_H_


namespace Stoneship
{

    class RecordReflector : public SubrecordFieldTracker
    {
    public:

        virtual ~RecordReflector();

        // interface
        virtual UID::Ordinal getCreatedBy() = 0;

        virtual void loadFromRecord(RecordAccessor &record);
        virtual void loadFromModifyRecord(RecordAccessor &record);
        virtual void storeToRecord(RecordBuilder &b);
        virtual void storeToModifyRecord(RecordBuilder &b);

    };

}


#endif /* INCLUDE_RECORDREFLECTOR_H_ */
