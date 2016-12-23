#ifndef SCREEN_H
#define SCREEN_H

#include <vector>

#include <osg/ref_ptr>
#include <osg/Group>
#include <osg/Projection>
#include <osg/MatrixTransform>
#include <osg/StateSet>

#include "Attachable.h"

namespace Stoneship
{
 
    class Subscreen;
    
    class Screen : public Attachable
    {
    public:
        Screen();
        virtual ~Screen();
        
        //override Attachable
        virtual void attachNodes(osg::Group *screenNode);
        virtual void detachNodes(osg::Group *screenNode);
        
        /**
         * Attaches a subscreen to this screen. This takes ownership of the pointer. The Subscreen is
         * automatically deallocated when this screen is destroyed.
         *
         * If the pointer subscreen is already present in the list of subscreens, calling this method will do 
         * nothing.
         *
         * subscreen may not be nullptr.
         */
        virtual void addSubscreen(Subscreen *subscreen);
        virtual void removeSubscreen(Subscreen *subscreen);
        virtual std::vector<Subscreen*> &getSubscreens();
        
        
    protected:
        
        osg::ref_ptr<osg::Group> mScreenGroup;
        osg::ref_ptr<osg::Geode> mScreenGeode;
        
        
    private:
        
        osg::ref_ptr<osg::Projection> mScreen2DProjection;
        osg::ref_ptr<osg::MatrixTransform> mScreenRelativeTransform;
        osg::ref_ptr<osg::StateSet> mScreenStateSet;
        
        std::vector<Subscreen*> mSubscreens;
        
    };
    
}

#endif // SCREEN_H
