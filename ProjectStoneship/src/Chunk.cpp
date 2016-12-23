
#include "Chunk.h"

#include "MasterGameFile.h"
#include "GameCache.h"
#include "IEntity.h"
#include "IEntityBase.h"

namespace Stoneship
{
    
    Chunk::Chunk(UID uid, IWorld *parentWorld)
    : mUID(uid)
    , mParentWorld(parentWorld)
    , mDisplayName("Overworld", Record::SUBTYPE_DISPLAY_NAME, this)
    , mGridPosition(osg::Vec2i(), Record::SUBTYPE_GRID, this)
    {
        
    }

    Chunk::~Chunk()
    {
        
    }
    
    UID Chunk::getCreatedUID() const
    {
        return getUID();
    }
    
    Record::Type Chunk::getRecordType() const
    {
        return Record::TYPE_CHUNK;
    }
    
    void Chunk::loadFromRecord(RecordAccessor &record, GameCache *gameCache)
    {
        RecordReflector::loadFromRecord(record, gameCache);
        
        record.getReaderForSubrecord(Record::SUBTYPE_DATA)
            >> mChunkFlags
            >> MGFDataReader::endr;
    }
    
    void Chunk::postLoad(RecordAccessor &last, RecordAccessor &surrounding, GameCache *gameCache)
    {
        RecordAccessor entityGroup = last.toIterator().next().getAccessor();
        ASSERT_RECORD_TYPE(entityGroup.getHeader().type == Record::TYPE_GROUP);
        ASSERT_RECORD_TYPE(entityGroup.getHeader().groupType == Record::TYPE_ENTITY);
        
        RecordIterator it = entityGroup.getChildIterator();
        while(it != entityGroup.getChildEnd())
        {
            RecordAccessor entRec = it.getAccessor();
            
            UID entityUID = UID(entRec.getGameFile()->getOrdinal(), entRec.getHeader().id);

            UID baseUID;
            Record::Type baseType;
            entRec.getReaderForSubrecord(Record::SUBTYPE_ENTITY)
                    >> baseUID
                    >> baseType
                    >> MGFDataReader::endr;

            IEntityBase *base = gameCache->getCachedElementOfType<IEntityBase>(baseUID, baseType);
            if(base == nullptr)
            {
                STONESHIP_EXCEPT(StoneshipException::RECORD_NOT_FOUND, "Base " + baseUID.toString() + " for Entity " + entityUID.toString() + " not found");
            }

            IEntity *entity = base->createEntity(entityUID);
            mEntities.push_back(entity);
            entity->loadFromRecord(entRec, gameCache);
            entity->spawn(mParentWorld);
            
            it++;
        }
    }
    
    UID Chunk::getUID() const
    {
        return mUID;
    }
    
    void Chunk::setGridPosition(const osg::Vec2i &v)
    {
        mGridPosition.set(v);
    }
    
    osg::Vec2i Chunk::getGridPosition() const
    {
        return mGridPosition.get();
    }
    
}
