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
#include "Attachable.h"
#include "IEntity.h"

namespace Stoneship
{

    class Chunk;

    class WorldOutdoor : public IWorld
    {
    public:

        WorldOutdoor(UID uid);
        ~WorldOutdoor();

        // override RecordReflector
        virtual Record::Type getRecordType() const { return Record::TYPE_OUTDOOR; };
        
        // override Attachable
        virtual void attachNodes(osg::Group *group, ResourceManager *resMan);
        virtual void detachNodes(osg::Group *group); 

        // override IWorld
        virtual String getWorldName() const;
        virtual const std::vector<IEntity*> &getLoadedEntities();
        virtual uint32_t getLoadedEntityCount() const;
        virtual void addEntity(IEntity *e);
        virtual void removeEntity(IEntity *entity);
        virtual void removeEntity(UID uid);

        // override IRecordReflector
        virtual void loadFromRecord(RecordAccessor &record, GameCache *gameCache);
        virtual void loadAttachment(RecordAccessor &attachment, GameCache *gameCache);
        virtual void storeToRecord(RecordBuilder &record);
        virtual void storeAttachment(RecordBuilder &surrounding);

        void setWorldName(const String &s);
        /**
         * @note WorldOutdoor owns it's loaded Chunks!
         */
        std::vector<Chunk*> &getLoadedChunks();
        

    private:

        SubrecordField<String> mWorldName;
        
        std::vector<Chunk*> mLoadedChunks;
    };

}


#endif /* INCLUDE_WORLDOUTDOOR_H_ */
