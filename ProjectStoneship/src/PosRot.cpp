

#include "PosRot.h"

#include "MGFDataWriter.h"
#include "MGFDataReader.h"

namespace Stoneship
{

    PosRot::PosRot()
    : mPos()
    , mRot()
    {

    }

    osg::Vec3d PosRot::getPosition() const
    {
        return mPos;
    }

    osg::Quat PosRot::getRotation() const
    {
        return mRot;
    }

    void PosRot::setPosition(const osg::Vec3d &v)
    {
        mPos = v;
    }

    void PosRot::setRotation(const osg::Quat &v)
    {
        mRot = v;
    }


    // serializers
    template <>
    MGFDataWriter &MGFDataWriter::operator << <PosRot>(const PosRot &v)
    {
        *this << v.getPosition() << v.getRotation();

        return *this;
    }

    template <>
    MGFDataReader &MGFDataReader::operator >> <PosRot>(PosRot &v)
    {
        osg::Vec3d pos;
        osg::Quat  rot;

        *this >> pos >> rot;

        v.setPosition(pos);
        v.setRotation(rot);

        return *this;
    }
}
