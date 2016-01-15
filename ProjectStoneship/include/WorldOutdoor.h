/*
 * WorldOutdoor.h
 *
 *  Created on: Dec 4, 2015
 *      Author: zalasus
 */

#ifndef INCLUDE_WORLDOUTDOOR_H_
#define INCLUDE_WORLDOUTDOOR_H_

#include "IWorld.h"

namespace Stoneship
{

    class WorldOutdoor : public IWorld
    {
    public:

        WorldOutdoor(UID uid);
        ~WorldOutdoor();

        String getWorldName() const;

        const std::vector<IEntity*> &getLoadedEntities();
        uint32_t getLoadedEntityCount() const;

        void removeEntity(IEntity *entity);
        void removeEntity(UID uid);

        void loadFromRecord(RecordAccessor &rec);


    private:

        String mWorldName;

        uint32_t **mChunkAddresses;

    };

}


#endif /* INCLUDE_WORLDOUTDOOR_H_ */
