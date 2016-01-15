/*
 * IRecordStorable.cpp
 *
 *  Created on: Jan 8, 2016
 *      Author: zalasus
 */

#include "IRecordLoadable.h"

#include "Exception.h"

namespace Stoneship
{

    IRecordLoadable::~IRecordLoadable()
    {
    }

    void IRecordLoadable::modifyFromRecord(RecordAccessor &rec, Record::ModifyType modType)
    {
        STONESHIP_EXCEPT(StoneshipException::UNSUPPSORTED, "Loadable does not support modification by record");
    }

}

