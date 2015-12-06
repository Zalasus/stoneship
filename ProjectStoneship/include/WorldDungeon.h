/*
 * WorldDungeon.h
 *
 *  Created on: Dec 3, 2015
 *      Author: zalasus
 */

#ifndef INCLUDE_WORLDDUNGEON_H_
#define INCLUDE_WORLDDUNGEON_H_

#include <vector>

#include "IWorld.h"
#include "Record.h"
#include "Types.h"
#include "String.h"

namespace Stoneship
{
    class IEntity;

    class WorldDungeon : public IWorld
    {
    public:

        WorldDungeon(UID uid);
        ~WorldDungeon();

        String getWorldName() const;

        const std::vector<IEntity*> &getLoadedEntities();
        uint32_t getLoadedEntityCount() const;

        void removeEntity(IEntity *entity);
        void removeEntity(UID uid);


        void loadFromRecord(RecordAccessor rec);


    private:

        String mDungeonName;
        std::vector<IEntity*> mEntities;

    };

}


#endif /* INCLUDE_WORLDDUNGEON_H_ */
