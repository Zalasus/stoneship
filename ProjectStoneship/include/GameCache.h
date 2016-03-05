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

        /**
         * Sets the policy that is used to allocate and deallocated cached elements. Possible values are defined in the enum GameCachePolicy
         * @l @c POLICY_PRELOAD_ALL          everything is loaded into memory at startup and never deallocated
         * @l @c POLICY_KEEP_ALL             data is loaded as needed but is never deallocated
         * @l @c POLICY_KEEP_NEEDED          data is loaded as needed and deallocated when not used anymore
         * @l @c POLICY_KEEP_RECENTLY_USED   data is loaded as needed and is deallocated after it hasn't been used for some time
         *
         * @relates collectGarbage
         * @param[in] policy The policy to be used
         */
        void setCachePolicy(GameCachePolicy policy);

        /**
         * Sets the number of calls to collectGarbage() needed to deallocate an unused cached element.
         * Only effective for cache policy POLICY_KEEP_RECENTLY_USED. A value of 1 causes the same behaviour as if POLICY_KEEP_NEEDED is beeing used.
         * A value of zero is invalid and will cause this method to throw. Default limit is 3.
         *
         * @param[in] i The amount of calls to collectGarbage() needed to deallocate an unused cached element.
         */
        void setLRULimit(uint32_t i);

        /**
         * Searches the cache for an entity base with given UID. If the requested base is not yet cached,
         * it is searched in the corresponding MGF and subsequently allocated, cached and returned. If a non-cached
         * base is also not found in it's MGF, this method will throw. The optional parameter type may be
         * provided to speed up searching the MGF for not yet cached bases.
         *
         * @param[in] uid   The UID of the desired entity base.
         * @param[in] type  The type of the desired base.
         * @returns The cached entity base with given UID.
         * @throws StoneshipException If non-cached bases can not be found in the MGF, IO errors occur while loading base from MGF etc.
         */
        IEntityBase *getBase(const UID &uid, Record::Type type = Record::TYPE_LOOKUP_ALL);

        /**
         * Takes ownership of base and caches it. Subsequent calls to getBase() will be able to yield this object
         * and calls to collectGarbage() or destruction of the GameCache object may deallocate it.
         * Behaviour for values of base that are already cached is undefined.
         *
         * @param[in] base The base to be cached.
         */
        void manageBase(IEntityBase *base);

        /**
         * @returns The amount of entity bases currently cached.
         */
        uint32_t getBaseCacheSize();

        /**
         * Searches the cache for a world with given UID. If the requested world is not yet cached,
         * it is searched in the corresponding MGF and subsequently allocated, cached and returned. If a non-cached
         * world is also not found in it's MGF, this method will throw. Note that since only one world is active at a
         * time, cached worlds may be deallocated as soon as another one is loaded. Therefore this method may trigger IO
         * action with every single call. The world cache's main purpose is to hold newly created worlds before they are written to an MGF.
         *
         * @param[in] uid   The UID of the desired world.
         * @returns The cached world with given UID.
         * @throws StoneshipException If non-cached worlds can not be found in the MGF, IO errors occur while loading world from MGF etc.
         */
        IWorld *getWorld(UID uid);

        /**
         * Takes ownership of world and caches it. Subsequent calls to getWorld() will be able to yield this object
         * and calls to collectGarbage() or destruction of the GameCache object may deallocate it.
         * Behaviour for values of world that are already cached is undefined.
         *
         * @param[in] world The base to be cached.
         */
        void manageWorld(IWorld *world);

        /**
         * Stores all cached elements to MGF that need to be stored.
         * Elements only need to be stored if they were newly created or created by a (volatile) SGF (have an ordinal of UID::SELF_REF_ORDINAL).
         * Any modifications made to existing elements need to be stored as MODIFY records, which is done in a
         * second pass performed by the storeCacheMods() method. This method creates all needed record groups
         * by itself. The passes writer object should be in an inter-record state (preferably right after MGF header) prior
         * to calling this method.
         * This method returns the amount of record groups it has written. This should be accumulated by whatever builds the
         * MGF to set the right amount of record groups in the header field.
         *
         * @param writer The writer that all record data is written to.
         * @returns The amount of record groups written by this method.
         * @throws StoneshipException In case of any IO errors
         */
        uint32_t storeCache(MGFDataWriter &writer);

        /**
         * Stores all modifications that were made to already existing elements as corresponding MODIFY records.
         * This method returns the amount of record groups it has written. Since at the moment only MODIFY records are
         * written, this either returns 1 or 0 in case no modifications were made.
         *
         * @relates storeCache
         * @param writer The writer that all record data is written to.
         * @returns The amount of record groups written by this method.
         * @throws StoneshipException In case of any IO errors
         */
        uint32_t storeCacheMods(MGFDataWriter &writer);

        /**
         * Checks all cached elements for their garbage state and deallocates them according to the given cache policy.
         * Only unneeded elements whose state can be recreated entirely by an MGF are collected (non-dirty, created by MGF not SGF).
         * Further restrictions may apply depeding on the chosen policy.
         *
         * @relates setCachePolicy
         */
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
