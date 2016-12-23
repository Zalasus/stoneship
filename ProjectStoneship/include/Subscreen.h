#ifndef SUBSCREEN_H
#define SUBSCREEN_H

#include <osg/Group>
#include <osg/Geode>
#include <osg/ref_ptr>

#include "Attachable.h"

namespace Stoneship
{
    
    class Subscreen : public Attachable
    {
    public:
        Subscreen();
        virtual ~Subscreen();

        //override Attachable
        virtual void attachNodes(osg::Group *screenNode);
        virtual void detachNodes(osg::Group *screenNode);
        
        
    protected:
        
        osg::ref_ptr<osg::Geode> mSubscreenGeode;

    private:
    };
    
}

#endif // SUBSCREEN_H
