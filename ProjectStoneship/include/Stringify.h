#ifndef STRINGIFY_H
#define STRINGIFY_H

#include "String.h"

namespace Stoneship
{

    template <typename T>
    class Stringify
    {
    public:
        
        /* Good idea, but GCC doesn't seem to like it :/
        
        static String operator()(const T &ref)
        {
            return "";
        }
        
        static String operator()(T &ref)
        {
            return (*this)(ref);
        }
        */
    };
    
}

#endif // STRINGIFY_H
