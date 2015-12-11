/*
 * ScreenWorld.cpp
 *
 *  Created on: Dec 11, 2015
 *      Author: zalasus
 */

#include "ScreenWorld.h"


namespace Stoneship
{

    ScreenWorld::ScreenWorld()
    : mDevice(Root::getSingleton()->getIrrlichtDevice()),
      mScm(mDevice->getSceneManager())
    {
    }

    void ScreenWorld::rebuildSceneGraph(irr::scene::ISceneNode *node)
    {
        node->removeAll();

        IWorld *world = Root::getSingleton()->getWorldManager()->getCurrentWorld();
        if(world != nullptr)
        {
            std::vector<IEntity*> &ent = world->getLoadedEntities();

            for(uint32_t i = 0; i < ent.size(); ++i)
            {
                IEntity *e = ent[i];

                if(e->getEntityType() & IEntity::ENTITYTYPE_WORLD)
                {
                    node->addChild(static_cast<EntityWorld*>(ent[i]));
                }
            }
        }

    }

    void ScreenWorld::render()
    {


    }

}


