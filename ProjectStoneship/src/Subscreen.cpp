
#include "Subscreen.h"

namespace Stoneship
{
    
    Subscreen::Subscreen()
    {
    }

    Subscreen::~Subscreen()
    {
    }
    
    void Subscreen::attachNodes(osg::Group *screenNode)
    {
        mSubscreenGeode = new osg::Geode();
        
        screenNode->addChild(mSubscreenGeode);
    }
    
    void Subscreen::detachNodes(osg::Group *screenNode)
    {
        screenNode->removeChild(mSubscreenGeode);
    }
}
