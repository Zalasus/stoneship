/*
 * WorldOutdoor.h
 *
 *  Created on: Dec 4, 2015
 *      Author: zalasus
 */

#ifndef INCLUDE_WORLDOUTDOOR_H_
#define INCLUDE_WORLDOUTDOOR_H_

#include "IWorld.h"
#include "SubrecordField.h"

namespace Stoneship
{

    class WorldOutdoor : public IWorld
    {
    public:

        WorldOutdoor(UID uid);
        ~WorldOutdoor();

        // override RecordReflector
        virtual Record::Type getRecordType() const { return Record::TYPE_OUTDOOR; };

        // override IWorld
        virtual String getWorldName() const;
        virtual const std::vector<IEntity*> &getLoadedEntities();
        virtual uint32_t getLoadedEntityCount() const;
        virtual void removeEntity(IEntity *entity);
        virtual void removeEntity(UID uid);

        // override IRecordReflector
        virtual void loadFromRecord(RecordAccessor &record);
        virtual void storeToRecord(RecordBuilder &record);

        void setWorldName(const String &s);


    private:

        SubrecordField<String> mWorldName;
    };

}


#endif /* INCLUDE_WORLDOUTDOOR_H_ */
