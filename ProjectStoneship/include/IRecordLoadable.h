/*
 * IRecordLoadable.h
 *
 *  Created on: Jan 8, 2016
 *      Author: zalasus
 */

#ifndef INCLUDE_IRECORDLOADABLE_H_
#define INCLUDE_IRECORDLOADABLE_H_

#include "Record.h"
#include "RecordAccessor.h"

namespace Stoneship
{

    /**
     * \brief Provides an interface for objects to be loaded from MGF records
     */
    class IRecordLoadable
    {
    public:

        virtual ~IRecordLoadable();

        virtual void loadFromRecord(RecordAccessor &rec) = 0;
        virtual void modifyFromRecord(RecordAccessor &rec, Record::ModifyType modType);

        virtual UID getUID() const = 0;

    };

}

#endif /* INCLUDE_IRECORDLOADABLE_H_ */
