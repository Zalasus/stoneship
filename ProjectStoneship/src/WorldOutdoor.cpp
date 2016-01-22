/*
 * WorldOutdoor.cpp
 *
 *  Created on: Dec 4, 2015
 *      Author: zalasus
 */

#include "WorldOutdoor.h"

namespace Stoneship
{


    WorldOutdoor::WorldOutdoor(UID uid)
    : IWorld(uid)
    {
    }

    WorldOutdoor::~WorldOutdoor()
    {
    }

    String WorldOutdoor::getWorldName() const
    {
        return mWorldName;
    }

    const std::vector<IEntity*> &WorldOutdoor::getLoadedEntities()
    {
        static std::vector<IEntity*> empty; //just for testing

        return empty;
    }

    uint32_t WorldOutdoor::getLoadedEntityCount() const
    {
        return 0; //testing...
    }

    void WorldOutdoor::removeEntity(IEntity *entity)
    {

    }

    void WorldOutdoor::removeEntity(UID uid)
    {

    }

    void WorldOutdoor::loadFromRecord(RecordAccessor &rec)
    {
        rec.getReaderForSubrecord(Record::SUBTYPE_DATA)
                .readBString(mWorldName);


        //create initial chunk map for faster loading without having to seek the MGF everytime
        rec = rec.getNextRecord();
        if(rec.getHeader().type == Record::TYPE_CHUNK_GROUP)
        {

        }
    }

}


