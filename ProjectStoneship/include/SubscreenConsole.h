#ifndef SUBSCREENCONSOLE_H
#define SUBSCREENCONSOLE_H

#include "Subscreen.h"

#include <osgText/Text>
#include <osg/Geometry>

#include "Logger.h"

namespace Stoneship
{
    
    class SubscreenConsole : public Subscreen, public ILoggerListener
    {
    public:
        
        virtual ~SubscreenConsole();
        
        //overrides Subscreen
        virtual void attachNodes(osg::Group *screenNode);
        virtual void detachNodes(osg::Group *screenNode);
        
        //overrides ILoggerListener
        virtual void onLog(const String &msg, Logger::LogLevel level);
        
    protected:

        void updateText();
    
    private:
        
        String mConsoleBacklog;
        String mConsolePrompt;
        
        osg::ref_ptr<osgText::Text> mConsoleBacklogGeom;
        osg::ref_ptr<osgText::Text> mConsolePromptGeom;
        osg::ref_ptr<osg::Geometry> mConsoleBGGeom;
        
    };

}

#endif // SUBSCREENCONSOLE_H
