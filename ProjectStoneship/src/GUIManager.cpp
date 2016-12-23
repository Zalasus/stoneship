
#include "GUIManager.h"

namespace Stoneship
{
    
    GUIManager::GUIManager()
    : mRootNode(nullptr)
    , mCurrentScreen(nullptr)
    {
    }

    GUIManager::~GUIManager()
    {
        if(mCurrentScreen != nullptr)
        {
            delete mCurrentScreen;
        }
    }
    
    void GUIManager::setRootNode(osg::Group *node)
    {
        mRootNode = node;
    }
    
    void GUIManager::setScreen(Screen *screen)
    {
        if(mCurrentScreen != nullptr)
        {
            mCurrentScreen->detachNodes(mRootNode);
            
            delete mCurrentScreen;
        }
        
        mCurrentScreen = screen;
        
        if(screen != nullptr)
        {
            screen->attachNodes(mRootNode);
        }
    }
    
    Screen *GUIManager::getScreen()
    {
        return mCurrentScreen;
    }
    
}
