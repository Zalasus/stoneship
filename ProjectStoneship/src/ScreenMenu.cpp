
#include "ScreenMenu.h"
#include "WorldOutdoor.h"
#include "Root.h"

#include <osgDB/ReadFile>

namespace Stoneship
{
    
    ScreenMenu::ScreenMenu()
    {
    }

    ScreenMenu::~ScreenMenu()
    {
    }
    
    void ScreenMenu::attachNodes(osg::Group *screenNode, ResourceManager *resMan)
    {
        Screen::attachNodes(screenNode);
        
        
        mMenuBGGeom = new osg::Geometry();
        osg::ref_ptr<osg::Vec3Array> bgVertices = new osg::Vec3Array;
        bgVertices->push_back(osg::Vec3( 0, 1, 0));
        bgVertices->push_back(osg::Vec3( 1, 1, 0));
        bgVertices->push_back(osg::Vec3( 1, 0, 0)); 
        bgVertices->push_back(osg::Vec3( 0, 0, 0)); 
        mMenuBGGeom->setVertexArray(bgVertices);
         
        osg::ref_ptr<osg::DrawElementsUInt> primitiveSet = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
        primitiveSet->push_back(3);
        primitiveSet->push_back(2);
        primitiveSet->push_back(1);
        primitiveSet->push_back(0);
        mMenuBGGeom->addPrimitiveSet(primitiveSet); 
        
        osg::ref_ptr<osg::Vec4Array> bgColors = new osg::Vec4Array;
        bgColors->push_back(osg::Vec4(1, 1, 1, 1));
        mMenuBGGeom->setColorArray(bgColors);
        mMenuBGGeom->setColorBinding(osg::Geometry::BIND_OVERALL);

        osg::ref_ptr<osg::Vec2Array> bgTexcoords = new osg::Vec2Array(4);
        (*bgTexcoords)[3].set(0.0f,0.0f);
        (*bgTexcoords)[2].set(1.0f,0.0f);
        (*bgTexcoords)[1].set(1.0f,1.0f);
        (*bgTexcoords)[0].set(0.0f,1.0f);
        mMenuBGGeom->setTexCoordArray(0,bgTexcoords);
        
        osg::ref_ptr<osg::Texture2D> bgTexture = new osg::Texture2D;
        bgTexture->setDataVariance(osg::Object::DYNAMIC);
        osg::Image* bgImage;
        bgImage = osgDB::readImageFile("res/tex/menu_bg.png");
        if(bgImage != nullptr)
        {
            bgTexture->setImage(bgImage);
        }
        
        osg::ref_ptr<osg::StateSet> bgState = new osg::StateSet();
        bgState->setTextureAttributeAndModes(0, bgTexture, osg::StateAttribute::ON);
        mMenuBGGeom->setStateSet(bgState);

        mScreenGeode->addDrawable(mMenuBGGeom);
        
        // and all this for a textured rectangle? *sigh*
        
        
        // next, load an outdoor scene for presentation
        
        
    }
    
    void ScreenMenu::detachNodes(osg::Group *screenNode)
    {
        Screen::detachNodes(screenNode);
    }
    
}
