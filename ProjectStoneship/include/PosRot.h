/*
 * PosRot.h
 *
 *  Created on: 6 Feb 2016
 *      Author: zalasus
 */

#ifndef INCLUDE_POSROT_H_
#define INCLUDE_POSROT_H_

#include <osg/Vec3d>
#include <osg/Quat>

namespace Stoneship
{
    class PosRot
    {
    public:

        PosRot();

        osg::Vec3d getPosition() const;
        osg::Quat getRotation() const;
        
        void setPosition(const osg::Vec3d &v);
        void setRotation(const osg::Quat &v);

    private:
    
        osg::Vec3d mPos;
        osg::Quat mRot;    

    };
}


#endif /* INCLUDE_POSROT_H_ */
