/*
 * PropertyBase.h
 *
 *  Created on: May 12, 2016
 *      Author: zalasus
 */

#ifndef INCLUDE_PROPERTYBASE_H_
#define INCLUDE_PROPERTYBASE_H_

namespace Stoneship
{

    class Property
    {
    public:

        typedef uint8_t PropertyType;
        static const PropertyType TYPE_INT = 0;
        static const PropertyType TYPE_LONG = 1;
        static const PropertyType TYPE_FLOAT = 3;
        static const PropertyType TYPE_DOUBLE = 4;
        static const PropertyType TYPE_STRING = 5;
        static const PropertyType TYPE_UID = 6;

        PropertyType getType();
        String getName();

        int32_t asInt();
        int64_t asLong();
        float asFloat();
        double asDouble();
        String asString();
        UID asUID();

    };

    class PropertyBase : public RecordReflector
    {
    public:

        PropertyBase();
        PropertyBase(PropertyBase &b);

        Property *getProperty(const String &name);



    };

}

#endif /* INCLUDE_PROPERTYBASE_H_ */
