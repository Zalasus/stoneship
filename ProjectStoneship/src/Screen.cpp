
#include "Screen.h"

#include <algorithm>

#include "Exception.h"
#include "Subscreen.h"

namespace Stoneship
{
    
    Screen::Screen()
    {
    }

    Screen::~Screen()
    {
        for(uint32_t i = 0; i < mSubscreens.size(); ++i)
        {
            delete mSubscreens[i];
        }
    }
    
    void Screen::attachNodes(osg::Group *rootNode, ResourceManager *resMan)
    {
        uint32_t screenWidth  = 1920; //TODO: Need to get these from some class that knows correct values
	    uint32_t screenHeight = 1080;
        
        mScreen2DProjection = new osg::Projection();
        mScreen2DProjection->setMatrix(osg::Matrix::ortho2D(0,screenWidth,0,screenHeight));
        
        mScreenRelativeTransform = new osg::MatrixTransform();
        mScreenRelativeTransform->setMatrix(osg::Matrix::scale(screenWidth,screenHeight,1)); // scale by Screen dimensions so HUD can use relative coordinates
        mScreenRelativeTransform->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
        
        mScreenStateSet = new osg::StateSet();
        mScreenStateSet->setMode(GL_BLEND,osg::StateAttribute::ON); // Turn blending on (so alpha texture looks right)
        mScreenStateSet->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF); // Disable depth testing
        mScreenStateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
        mScreenStateSet->setRenderBinDetails(11, "RenderBin"); // Need to make sure this geometry is drawn last. RenderBins are handled in numerical order so set bin number to 11
        
        mScreenGroup = new osg::Group();
        mScreenGroup->setStateSet(mScreenStateSet);
        
        mScreenGeode = new osg::Geode();
        mScreenGroup->addChild(mScreenGeode);
                
        mScreenRelativeTransform->addChild(mScreenGroup);
        mScreen2DProjection->addChild(mScreenRelativeTransform);
        
        rootNode->addChild(mScreen2DProjection);
    }
    
    void Screen::detachNodes(osg::Group *rootNode)
    {
        rootNode->removeChild(mScreen2DProjection);
    }
    
    void Screen::addSubscreen(Subscreen *subscreen)
    {
        ASSERT_ARG(subscreen != nullptr);
        
        
        std::vector<Subscreen*>::iterator it = std::find(mSubscreens.begin(), mSubscreens.end(), subscreen);
        
        if(it != mSubscreens.end())
        {
            // subscreen already present. get outta here
            return;
        }
        
        subscreen->attachNodes(mScreenGroup);
        
        mSubscreens.push_back(subscreen);
    }
    
    void Screen::removeSubscreen(Subscreen *subscreen)
    {
        ASSERT_ARG(subscreen != nullptr);
        
        
        std::vector<Subscreen*>::iterator it = std::find(mSubscreens.begin(), mSubscreens.end(), subscreen);
        
        if(it != mSubscreens.end())
        {
            subscreen->detachNodes(mScreenGroup);
            
            mSubscreens.erase(it);
        }
    }
    
    std::vector<Subscreen*> &Screen::getSubscreens()
    {
        return mSubscreens;
    }
    
}
