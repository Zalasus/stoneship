/**
 * This file contains serializers for classes provided by osg.
 *  I know osg provides it's own serializers (WTF, that shit took like 80% of the compilation time!),
 *  but to keep things internal we write our own specializations for the MGF reader/writer.
 */

#include "MGFSerializers_OSG.h"

#include "MGFDataWriter.h"
#include "MGFDataReader.h"

#include <osg/Vec2i>
#include <osg/Vec3d>
#include <osg/Quat>

namespace Stoneship
{
    
    template <>
    MGFDataWriter &MGFDataWriter::operator << <osg::Vec2i>(const osg::Vec2i &v)
    {
        *this << v.x() << v.y();

        return *this;
    }
    
    template <>
	MGFDataReader &MGFDataReader::operator >> <osg::Vec2i>(osg::Vec2i &v)
	{
	    int32_t x;
	    int32_t y;
	    
		*this >> x >> y;
		
		v.set(x,y);

		return *this;
	}
    

    template <>
    MGFDataWriter &MGFDataWriter::operator << <osg::Vec3d>(const osg::Vec3d &v)
    {
        *this << v.x() << v.y() << v.z();

        return *this;
    }

    template <>
    MGFDataReader &MGFDataReader::operator >> <osg::Vec3d>(osg::Vec3d &v)
    {
        double x;
        double y;
        double z;

        *this >> x >> y >> z;

        v.set(x, y, z);

        return *this;
    }



    template <>
    MGFDataWriter &MGFDataWriter::operator << <osg::Quat>(const osg::Quat &v)
    {
        *this << v.x() << v.y() << v.z() << v.w();

        return *this;
    }

    template <>
    MGFDataReader &MGFDataReader::operator >> <osg::Quat>(osg::Quat &v)
    {
        double x;
        double y;
        double z;
        double w;

        *this >> x >> y >> z >> w;

        v.set(x, y, z, w);

        return *this;
    }

}
