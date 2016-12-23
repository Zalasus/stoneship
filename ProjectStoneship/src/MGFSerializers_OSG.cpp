/**
 * This file contains serializers for classes provided by osg.
 *  I know osg provides it's own serializers (WTF, that shit took like 80% of the compilation time!),
 *  but to keep things internal we write our own specializations for the MGF reader/writer.
 */

#include "MGFSerializers_OSG.h"

#include "MGFDataWriter.h"
#include "MGFDataReader.h"

#include <osg/Vec2i>

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
    
}
