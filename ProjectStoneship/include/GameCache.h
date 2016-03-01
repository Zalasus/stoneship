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

        enum GameCachePolicy
        {
            POLICY_PRELOAD_ALL,           // everything is loaded into memory at startup and never deallocated
            POLICY_KEEP_ALL,              // data is loaded as needed but is never deallocated
            POLICY_KEEP_NEEDED,           // data is loaded as needed and deallocated when not used anymore
            POLICY_KEEP_RECENTLY_USED     // data is loaded as needed and is deallocated after it hasn't been used for some time
        };

        GameCache(Root *root);
        ~GameCache();

        void setCachePolicy(GameCachePolicy policy);
        /**
         * Sets the number of calls to collectGarbage() needed to deallocate an unused cached element.
         * Only effective for cache policy POLICY_KEEP_RECENTLY_USED. A value of 1 causes the same behaviour as if POLICY_KEEP_NEEDED is beeing used.
         * A value of zero is invalid and will cause this method to throw. Default limit is 3
         */
        void setLRULimit(uint32_t i);

        IEntityBase *getBase(const UID &uid, Record::Type type = Record::TYPE_LOOKUP_ALL);
        IEntityBase *manageBase(IEntityBase *base);
        uint32_t getBaseCacheSize();

        IWorld *getWorld(UID uid);
        IWorld *manageWorld(IWorld *world);

        uint32_t storeCache(MGFDataWriter &writer);
        uint32_t storeCacheMods(MGFDataWriter &writer);

        void collectGarbage();

    private:

        Root *mRoot;

        GameCachePolicy mPolicy;
        uint32_t mLRULimit;

        std::vector<IWorld*> mWorldCache;
        std::vector<IEntityBase*> mBaseCache;
    };

}


#endif /* INCLUDE_GAMECACHE_H_ */
