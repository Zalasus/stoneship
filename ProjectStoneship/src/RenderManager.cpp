/*
 * RenderManager.cpp
 *
 *  Created on: Dec 8, 2015
 *      Author: zalasus
 */

#include "RenderManager.h"

#include <irrlicht.h>

#include "Root.h"
#include "IScreen.h"
#include "Logger.h"

namespace Stoneship
{

    RenderManager::RenderManager(Root *root)
    : mRoot(root),
      mCurrentScreen(nullptr)
    {
    }

    RenderManager::~RenderManager()
    {
        if(mCurrentScreen != nullptr)
        {
            delete mCurrentScreen;
        }
    }

    void RenderManager::setScreen(IScreen *screen)
    {
        if(mCurrentScreen != nullptr)
        {
            mCurrentScreen->end();

            delete mCurrentScreen;
        }

        mCurrentScreen = screen;
    }

    void RenderManager::startRendering()
    {
        irr::IrrlichtDevice *device = mRoot->getIrrlichtDevice();
        irr::video::IVideoDriver *driver = device->getVideoDriver();
        irr::scene::ISceneManager *scm = device->getSceneManager();

        bool focus = false;

        while(device->run())
        {
            if(!device->isWindowActive())
            {
                device->yield();
                if(focus)
                {
                    focus = false;
                    Logger::info("Render window lost focus");
                }
                continue;
            }

            if(!focus)
            {
                focus = true;
                Logger::info("Render window gained focus");
            }



            driver->beginScene(true, true, irr::video::SColor(0)); // fill frame buffer with black

            if(mCurrentScreen != nullptr)
            {
                mCurrentScreen->render();
            }

            scm->drawAll();

            if(mRoot->getOptions().getLogFPS() && device->getTimer()->getTime() % 1000 == 0)
            {
                Logger::info(String("Current FPS is: ") + driver->getFPS());
            }

            driver->endScene();
        }
    }

    void RenderManager::rebuildSceneGraph()
    {
        irr::IrrlichtDevice *device = mRoot->getIrrlichtDevice();
        irr::scene::ISceneManager *scm = device->getSceneManager();

        if(mCurrentScreen != nullptr)
        {
             mCurrentScreen->rebuildSceneGraph(scm->getRootSceneNode());
        }
    }

}

