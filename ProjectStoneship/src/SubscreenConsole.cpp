
#include "SubscreenConsole.h"

namespace Stoneship
{
    
    SubscreenConsole::~SubscreenConsole()
    {
        Logger::getDefaultLogger().removeListener(this);
    }
    
    void SubscreenConsole::attachNodes(osg::Group *screenNode, ResourceManager *resMan)
    {
        Subscreen::attachNodes(screenNode);
        
        mConsoleBacklogGeom = new osgText::Text();
        mConsoleBacklogGeom->setAlignment(osgText::Text::LEFT_TOP);
        mConsoleBacklogGeom->setFont("/usr/share/fonts/TTF/LiberationMono-Regular.ttf");
        mConsoleBacklogGeom->setCharacterSize(16);
        mConsoleBacklogGeom->setAxisAlignment(osgText::Text::SCREEN);
        mConsoleBacklogGeom->setPosition(osg::Vec3(0.5,0.5,0.5));
        mConsoleBacklogGeom->setColor(osg::Vec4(255, 0, 255, 0.5));
        mSubscreenGeode->addDrawable(mConsoleBacklogGeom);
        mConsoleBacklog = "Stoneship Console\n"
                          "=================\n\n";
        
        updateText();
        
        double consoleBGLowerLimit = 0.8;
        float consoleAlpha = 0.6;
        
        mConsoleBGGeom = new osg::Geometry();
        osg::ref_ptr<osg::Vec3Array> bgVertices = new osg::Vec3Array;
        bgVertices->push_back(osg::Vec3( 0, 1, 0));
        bgVertices->push_back(osg::Vec3( 1, 1, 0));
        bgVertices->push_back(osg::Vec3( 1, consoleBGLowerLimit, 0)); 
        bgVertices->push_back(osg::Vec3( 0, consoleBGLowerLimit, 0)); 
        mConsoleBGGeom->setVertexArray(bgVertices);
         
        osg::ref_ptr<osg::DrawElementsUInt> primitiveSet = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
        primitiveSet->push_back(3);
        primitiveSet->push_back(2);
        primitiveSet->push_back(1);
        primitiveSet->push_back(0);
        mConsoleBGGeom->addPrimitiveSet(primitiveSet); 
        
        osg::Vec4Array* bgColors = new osg::Vec4Array;
        bgColors->push_back(osg::Vec4(0, 0, 0, consoleAlpha));
        
        mConsoleBGGeom->setColorArray(bgColors);
        mConsoleBGGeom->setColorBinding(osg::Geometry::BIND_OVERALL);
        
        mSubscreenGeode->addDrawable(mConsoleBGGeom);
        
        Logger::getDefaultLogger().addListener(this);
    }
    
    void SubscreenConsole::detachNodes(osg::Group *screenNode, ResourceManager *resMan)
    {
        Subscreen::detachNodes(screenNode);
        
        Logger::getDefaultLogger().removeListener(this);
    }
    
    void SubscreenConsole::onLog(const String &msg, Logger::LogLevel level)
    {
        mConsoleBacklog += msg + "\n";
        
        updateText();
    }
    
    void SubscreenConsole::updateText()
    {
        mConsoleBacklogGeom->setText(mConsoleBacklog);
    }
    
}
