/*
 * ScreenWorld.h
 *
 *  Created on: Dec 10, 2015
 *      Author: zalasus
 */

#ifndef INCLUDE_SCREENWORLD_H_
#define INCLUDE_SCREENWORLD_H_

#include "IScreen.h"

namespace Stoneship
{

    class ScreenWorld : public IScreen
    {
    public:

        void rebuildSceneGraph(irr::scene::ISceneNode *node);

        void render();


    private:

        irr::IrrlichtDevice *mDevice;
        irr::scene::ISceneManager *mScm;

    };

}


#endif /* INCLUDE_SCREENWORLD_H_ */
