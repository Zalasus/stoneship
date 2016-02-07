/*
 * Root.cpp
 *
 *  Created on: Nov 17, 2015
 *      Author: zalasus
 */


#include "Root.h"

#include "MGFManager.h"
#include "EntityManager.h"
#include "WorldManager.h"
#include "ResourceManager.h"
#include "EventPipeline.h"
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
	  mEventPipeline(nullptr)
	{
	    if(smSingleton != nullptr)
	    {
	        Logger::warn("Created second instance of Root object.");

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
		delete mEventPipeline;
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

	EventPipeline *Root::getEventPipeline()
	{
	    if(mEventPipeline == nullptr)
	    {
	        mEventPipeline = new EventPipeline();
	    }

	    return mEventPipeline;
	}

	void Root::loadAllMGFs()
	{
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
	}

	void Root::run()
	{
	    // add default resource path
	    getResourceManager()->addResourcePath(STONESHIP_DEFAULT_RESOURCE_PATH, ResourceManager::PATH_FILESYSTEM);


	    // now that all MGFs are loaded, we need to find an entry point
	    //getWorldManager()->enterWorld(UID(0xA));


	    // we are done here. give control back to implementation
	    Logger::info("Stop signal received. Shutting down engine.");
	}



	Root *Root::smSingleton = nullptr;

	Root *Root::getSingleton()
	{
	    if(smSingleton == nullptr)
	    {
	        Logger::warn("Created Root singleton using singleton method. ATM, this creates a memory leak and should be avoided.");

	        smSingleton = new Root(); //TODO: technically, this method creates a memory leak. might be worth checking out
	    }

	    return smSingleton;
	}


}
