/*
 * ModTracked.cpp
 *
 *  Created on: Jan 21, 2016
 *      Author: zalasus
 */

#include "SubrecordField.h"

namespace Stoneship
{

    SubrecordFieldS::SubrecordFieldS(RecordReflector *reflector)
    : mDirty(false)
    {
        if(reflector != nullptr)
        {
            reflector->_registerForReflection(this);
        }
    }

    SubrecordFieldS::~SubrecordFieldS()
    {
    }

    bool SubrecordFieldS::isDirty() const
    {
        return mDirty;
    }

    void SubrecordFieldS::setDirty(bool dirty)
    {
        mDirty = dirty;
    }

}
