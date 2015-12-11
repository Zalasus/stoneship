/*
 * RenderManager.h
 *
 *  Created on: Dec 7, 2015
 *      Author: zalasus
 */

#ifndef INCLUDE_RENDERMANAGER_H_
#define INCLUDE_RENDERMANAGER_H_


namespace Stoneship
{

    class Root;
    class IScreen;

    class RenderManager
    {
    public:

        RenderManager(Root *root);
        ~RenderManager();

        void setScreen(IScreen *screen);

        void startRendering();

        void rebuildSceneGraph();

    private:

        Root *mRoot;
        IScreen *mCurrentScreen;

    };

}


#endif /* INCLUDE_RENDERMANAGER_H_ */
