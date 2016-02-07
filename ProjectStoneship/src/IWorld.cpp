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

    UID IWorld::getCreatedUID() const
    {
        return mUID;
    }

    UID IWorld::getUID() const
    {
        return mUID;
    }

}


