/*
 * SASEntities.h
 *
 *  Created on: Feb 4, 2016
 *      Author: zalasus
 */

#ifndef INCLUDE_SAS_SASENTITIES_H_
#define INCLUDE_SAS_SASENTITIES_H_

#include "IEntity.h"

#include "sas/SASEntityBases.h"


namespace Stoneship
{

    class EntityContainer : public EntityWorld
    {
    public:

        EntityContainer(UID uidOfEntity, EntityBase_Container *base); //uidOfEntity is the UID of the reference, not the referenced base!!!
        EntityContainer(const EntityWorld &e) = delete; //don't copy me!
        ~EntityContainer();

        // overrides EntityWorld

        // overrides IRecordReflector
        virtual void loadFromRecord(RecordAccessor &record);
        virtual void loadFromModifyRecord(RecordAccessor &record);
        virtual void storeToRecord(RecordBuilder &record);
        virtual void storeToModifyRecord(RecordBuilder &record);

        Inventory &getInventory();


    private:

        Inventory mInventory;

    };

}


#endif /* INCLUDE_SAS_SASENTITIES_H_ */
