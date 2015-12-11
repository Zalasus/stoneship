/*
 * Root.cpp
 *
 *  Created on: Nov 17, 2015
 *      Author: zalasus
 */


#include "Root.h"

#include <irrlicht.h>

#include "MGFManager.h"
#include "EntityManager.h"
#include "WorldManager.h"
#include "ResourceManager.h"
#include "EventPipeline.h"
#include "RenderManager.h"
#include "Logger.h"
#include "Exception.h"
#include "StoneshipDefines.h"

namespace Stoneship
{

	Root::Root()
	: mOptions(this),
	  mMGFManager(nullptr),
	  mEntityManager(nullptr),
	  mWorldManager(nullptr),
	  mResourceManager(nullptr),
	  mRenderManager(nullptr),
	  mEventPipeline(nullptr),
	  mIrrlichtDevice(nullptr)
	{
	    if(smSingleton != nullptr)
	    {
	        Logger::warn("Created second instance of Root object. This indicates an error in the engine");

	    }else
	    {
	        smSingleton = this; //catch any instantiations not made by the singleton method
	    }
	}

	Root::~Root()
	{
		delete mMGFManager;
		delete mEntityManager;
		delete mWorldManager;
		delete mResourceManager;
		delete mRenderManager;
		delete mEventPipeline;
		delete mIrrlichtDevice;
	}

	Options &Root::getOptions()
	{
		return mOptions;
	}

	MGFManager *Root::getMGFManager()
	{
		if(mMGFManager == nullptr)
		{
			mMGFManager = new MGFManager(this);
		}

		return mMGFManager;
	}

	EntityManager *Root::getEntityManager()
	{
		if(mEntityManager == nullptr)
		{
			mEntityManager = new EntityManager(this);
		}

		return mEntityManager;
	}

	WorldManager *Root::getWorldManager()
	{
		if(mWorldManager == nullptr)
		{
			mWorldManager = new WorldManager(this);
		}

		return mWorldManager;
	}

	ResourceManager *Root::getResourceManager()
	{
		if(mResourceManager == nullptr)
		{
			mResourceManager = new ResourceManager(this);
		}

		return mResourceManager;
	}

	RenderManager *Root::getRenderManager()
	{
	    if(mRenderManager == nullptr)
	    {
	        mRenderManager = new RenderManager(this);
	    }

	    return mRenderManager;
	}

	EventPipeline *Root::getEventPipeline()
	{
	    if(mEventPipeline == nullptr)
	    {
	        mEventPipeline = new EventPipeline();
	    }

	    return mEventPipeline;
	}

	irr::IrrlichtDevice *Root::getIrrlichtDevice()
	{
	    if(mIrrlichtDevice == nullptr)
	    {
	        mIrrlichtDevice = _createRendererDevice();

	        if(mIrrlichtDevice == nullptr)
	        {
	            STONESHIP_EXCEPT(StoneshipException::RENDERER_ERROR, "Could not create renderer device");
	        }
	    }

	    return mIrrlichtDevice;
	}

	void Root::run()
	{
	    getIrrlichtDevice(); // create renderer first. it's not neccessary to continue booting up when no renderer could be created

	    getResourceManager()->addResourcePath("res/", UID::SELF_REF_ORDINAL, ResourceManager::PATH_FILESYSTEM, ResourceManager::PRIORITY_BEFORE_DEFAULT);

	    // load MGFs from config file
	    const std::vector<IniFile::IniEntry> &mgfEntries = getOptions().getIniFile().getEntriesInSection("mgf");
	    for(uint32_t i = 0; i < mgfEntries.size(); ++i)
	    {
	        try
	        {
	            Logger::info("Loading MGF '" + mgfEntries[i].value + "'");

	            getMGFManager()->loadMGF(mgfEntries[i].value);

	        }catch(StoneshipException &e)
	        {
	            STONESHIP_EXCEPT(e.getType(), "Error while loading MGF " + mgfEntries[i].value + " from config file: " + e.getMessage());
	        }
	    }

	    Logger::info(String("Loaded ") + getMGFManager()->getLoadedMGFCount() + " MGF(s)");

	    // init default window caption
	    getIrrlichtDevice()->setWindowCaption(STONESHIP_DEFAULT_WINDOW_CAPTION_WIDE);

	    getRenderManager()->startRendering(); // atm there's not much to it. might extend this function in the future or omit it completely
	}

	irr::IrrlichtDevice *Root::_createRendererDevice()
	{
	    irr::core::dimension2d<uint32_t> res(getOptions().getResolutionX(), getOptions().getResolutionY());
	    uint32_t bits = getOptions().getIniFile().getValueAsInt("graphics", "bits", 32);

	    return irr::createDevice(irr::video::EDT_OPENGL, res, bits, getOptions().getFullscreen(), true, getOptions().getVsync()); //TODO: add options here
	}


	Root *Root::smSingleton = nullptr;

	Root *Root::getSingleton()
	{
	    if(smSingleton == nullptr)
	    {
	        smSingleton = new Root();
	    }

	    return smSingleton;
	}


}
