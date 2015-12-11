/*
 * IScreen.h
 *
 *  Created on: Dec 9, 2015
 *      Author: zalasus
 */

#ifndef INCLUDE_GUI_ISCREEN_H_
#define INCLUDE_GUI_ISCREEN_H_

namespace Stoneship
{

    class RenderManager;

    class IScreen
    {
    public:

        virtual ~IScreen();

        virtual void begin(RenderManager *rm);
        virtual void end();

        virtual void rebuildSceneGraph(irr::scene::ISceneNode *node) = 0;

        virtual void render() = 0;


    protected:

        RenderManager *mRenderManager;

    };

}


#endif /* INCLUDE_GUI_ISCREEN_H_ */
