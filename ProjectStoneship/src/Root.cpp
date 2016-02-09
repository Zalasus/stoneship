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

namespace Stoneship
{

	Root::Root()
	: mOptions(this),
	  mMGFManager(this),
	  mResourceManager(this),
	  mGameCache(this),
	  mEventPipeline()
	{
	    if(smSingleton != nullptr)
	    {
	        Logger::warn("Created second instance of Root object.");

	    }else
	    {
	        smSingleton = this; // catch any instantiations not made by the singleton method
	    }
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
        const std::vector<IniFile::IniEntry> &mgfEntries = getOptions().getIniFile().getEntriesInSection("mgf");
        for(uint32_t i = 0; i < mgfEntries.size(); ++i)
        {
            try
            {
                Logger::info("Loading MGF '" + mgfEntries[i].value + "'");

                getMGFManager().loadMGF(mgfEntries[i].value);

            }catch(StoneshipException &e)
            {
                STONESHIP_EXCEPT(e.getType(), "Error while loading MGF " + mgfEntries[i].value + " from config file: " + e.getMessage());
            }
        }

        Logger::info(String("Loaded ") + getMGFManager().getLoadedMGFCount() + " MGF(s)");
	}

	void Root::run()
	{

	    // we need to find an entry point


	    // we are done here. give control back to implementation
	    Logger::info("Stop signal received. Shutting down engine.");
	}



	Root *Root::smSingleton = nullptr;

	Root *Root::getSingleton()
	{
	    if(smSingleton == nullptr)
	    {
	        Logger::warn("Initial Root singleton created using singleton method. ATM, this creates a memory leak and should be avoided.");

	        smSingleton = new Root(); //TODO: technically, this method creates a memory leak. might be worth checking out
	    }

	    return smSingleton;
	}


}
