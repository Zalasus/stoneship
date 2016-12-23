#ifndef ATTACHABLE_H
#define ATTACHABLE_H

#include "ResourceManager.h"

#include <osg/Group>

namespace Stoneship
{
    
    class Attachable
    {
    public:
        virtual ~Attachable();
        
        //interface
        /**
         * @param group The group to which the Attachable should attach all it's nodes and subgroups
         * @param resMan A ResourceManager instance that can be used to aquire models etc
         */
        virtual void attachNodes(osg::Group *group, ResourceManager *resMan) = 0;
        virtual void detachNodes(osg::Group *group) = 0;
        

    protected:
        

    private:
        
        
    };
    
}

#endif // ATTACHABLE_H
