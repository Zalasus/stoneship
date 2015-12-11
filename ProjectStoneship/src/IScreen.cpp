/*
 * IScreen.cpp
 *
 *  Created on: Dec 10, 2015
 *      Author: zalasus
 */

#include "IScreen.h"

#include "RenderManager.h"

namespace Stoneship
{

    void IScreen::begin(RenderManager *rm)
    {
        mRenderManager = rm;
    }

    void IScreen::end()
    {

    }

}


