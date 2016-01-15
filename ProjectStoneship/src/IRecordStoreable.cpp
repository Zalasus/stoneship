/*
 * IRecordStoreable.cpp
 *
 *  Created on: Jan 14, 2016
 *      Author: zalasus
 */

#include "IRecordStoreable.h"

namespace Stoneship
{

    IRecordStoreable::IRecordStoreable()
    : mRecordDirty(true)
    {
    }

    IRecordStoreable::~IRecordStoreable()
    {
    }

    bool IRecordStoreable::isDirty()
    {
        return mRecordDirty;
    }

    void IRecordStoreable::setDirty(bool b)
    {
        mRecordDirty = b;
    }

}



