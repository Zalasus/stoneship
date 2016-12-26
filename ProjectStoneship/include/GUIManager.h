#ifndef GUIMANAGER_H
#define GUIMANAGER_H

#include "Screen.h"

#include <osg/Group>
#include <osg/ref_ptr>

namespace Stoneship
{
    
    class GUIManager
    {
    public:
        GUIManager(ResourceManager *resMan);
        ~GUIManager();
        
        void setRootNode(osg::Group *node);

        /**
         * Sets and initializes the screen to be displayed. Takes ownership of passed pointer.
         * The Screen instance is deallocated once another Screen is loaded or the GUIManager
         * is destroyed. 
         *
         * Values of nullptr for screen are valid and result in no Screen beeing rendered.
         */
        void setScreen(Screen *screen);
        Screen *getScreen();
        

    private:
        
        ResourceManager *mResMan;
        osg::ref_ptr<osg::Group> mRootNode;
        Screen *mCurrentScreen;
        
    };
    
}

#endif // GUIMANAGER_H
