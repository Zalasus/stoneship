/*
 * GameCache.h
 *
 *  Created on: 7 Feb 2016
 *      Author: zalasus
 */

#ifndef INCLUDE_GAMECACHE_H_
#define INCLUDE_GAMECACHE_H_

#include <vector>

#include "Record.h"

namespace Stoneship
{

    class Root;
    class IEntityBase;
    class MGFDataWriter;
    class RecordReflector;
    class IWorld;

    class GameCache
    {
    public:

        GameCache(Root *root);
        ~GameCache();

        IEntityBase *getBase(const UID &uid, Record::Type type = Record::TYPE_LOOKUP_ALL);
        IEntityBase *manageBase(IEntityBase *base);
        uint32_t getBaseCacheSize();

        IWorld *getWorld(UID uid);
        IWorld *manageWorld(IWorld *world);

        uint32_t storeCache(MGFDataWriter &writer);
        uint32_t storeCacheMods(MGFDataWriter &writer);

        void collectGarbage();

    private:

        void _buildAndSortCombinedCache();

        Root *mRoot;

        std::vector<IWorld*> mWorldCache;
        std::vector<IEntityBase*> mBaseCache;
    };

}


#endif /* INCLUDE_GAMECACHE_H_ */
