
#include "GUIManager.h"

namespace Stoneship
{
    
    GUIManager::GUIManager(ResourceManager *resMan)
    : mResMan(resMan)
    , mRootNode(nullptr)
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
            screen->attachNodes(mRootNode, mResMan);
        }
    }
    
    Screen *GUIManager::getScreen()
    {
        return mCurrentScreen;
    }
    
}
