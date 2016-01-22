/*
 * IWorld.cpp
 *
 *  Created on: Dec 3, 2015
 *      Author: zalasus
 */

#include "IWorld.h"

namespace Stoneship
{

    IWorld::IWorld(UID uid)
    : mUID(uid)
    {
    }

    IWorld::~IWorld()
    {
    }

    UID::Ordinal IWorld::getCreatedBy()
    {
        return mUID.ordinal;
    }

    UID IWorld::getUID() const
    {
        return mUID;
    }

}


