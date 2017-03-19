/*
 * Root.cpp
 *
 *  Created on: Nov 17, 2015
 *      Author: zalasus
 */


#include "Root.h"

#include "MGFManager.h"
#include "GameCache.h"
#include "ResourceManager.h"
#include "EventPipeline.h"
#include "Logger.h"
#include "Exception.h"
#include "StoneshipDefines.h"
#include "Screen.h"
#include "ScreenMenu.h"
#include "SubscreenConsole.h"

#include <osg/MatrixTransform>
#include <osg/Group>
#include <osg/Geode>
#include <osg/StateSet>
#include <osg/Projection>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osg/ShapeDrawable>
#include <osg/Shape>
#include <osgText/Text>

namespace Stoneship
{

	Root::Root(Options &options)
	: mOptions(options)
	, mResourceManager()
	, mMGFManager(&mResourceManager)
	, mGameCache(&mMGFManager)
	, mEventPipeline()
	, mGuiManager(&mResourceManager)
	{
	}

	Root::~Root()
	{
	}

	Options &Root::getOptions()
	{
		return mOptions;
	}

	MGFManager &Root::getMGFManager()
	{
		return mMGFManager;
	}

	ResourceManager &Root::getResourceManager()
	{
		return mResourceManager;
	}

	GameCache &Root::getGameCache()
	{
		return mGameCache;
	}

	EventPipeline &Root::getEventPipeline()
	{
	    return mEventPipeline;
	}

	void Root::loadAllMGFs()
	{
	    // load MGFs from config file
        std::vector<String> &mgfEntries = mOptions.getMGFList();
        for(uint32_t i = 0; i < mgfEntries.size(); ++i)
        {
            try
            {
                Logger::info("Loading MGF '" + mgfEntries[i] + "'");

                getMGFManager().loadMGF(mgfEntries[i]);

            }catch(StoneshipException &e)
            {
                STONESHIP_EXCEPT(e.getType(), "Error while loading MGF " + mgfEntries[i] + " from config file: " + e.getMessage());
            }
        }

        Logger::info(String("Loaded ") + getMGFManager().getLoadedMGFCount() + " MGF(s)");
	}

	void Root::run()
	{
	    osg::ref_ptr<osg::Group> rootNode = new osg::Group();
	    mGuiManager.setRootNode(rootNode);
	    
	    //Screen *plainScreen = new Screen();
	    //mGuiManager.setScreen(plainScreen);
        //plainScreen->addSubscreen(new SubscreenConsole());
	    
	    Logger::info("Creating menu...");
	    
	    ScreenMenu *menu = new ScreenMenu();
	    mGuiManager.setScreen(menu);
	    
	    osg::ref_ptr<osg::Geode> cylinder = new osg::Geode();
	    osg::ref_ptr<osg::Cylinder> cylinderShape = new osg::Cylinder(osg::Vec3(0,0,0), 1, 4);
	    osg::ref_ptr<osg::ShapeDrawable> cylinderDrawable = new osg::ShapeDrawable(cylinderShape);
	    cylinderDrawable->setColor(osg::Vec4(0.2, 0.7, 0.5, 1));
	    cylinder->addDrawable(cylinderDrawable);
	    rootNode->addChild(cylinder);
	    
	    Logger::info("Everything set up, running viwer...");
	    
	    osgViewer::Viewer viewer;
	    viewer.getCamera()->setClearColor(osg::Vec4(0,0,0,1));
	    viewer.setSceneData(rootNode);
	    viewer.run();
	    
	    // we are done here
	    Logger::info("Stop signal received. Shutting down engine.");
	}


}
