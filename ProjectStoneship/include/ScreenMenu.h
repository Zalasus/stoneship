#ifndef SCREENMENU_H
#define SCREENMENU_H

#include "Screen.h"

#include <osgText/Text>
#include <osg/Geometry>

namespace Stoneship
{
    
    class ScreenMenu : public Screen
    {
    public:
        ScreenMenu();
        virtual ~ScreenMenu();

        //overrides Screen
        virtual void attachNodes(osg::Group *screenNode, ResourceManager *resMan);
        virtual void detachNodes(osg::Group *screenNode);
        
        
    protected:

    
    
    private:
        
        osg::ref_ptr<osg::Geometry> mMenuBGGeom;
        
    };

}

#endif // SCREENMENU_H
