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

	Root::Root(Options &options)
	: mOptions(options)
	, mMGFManager(this)
	, mResourceManager(this)
	, mGameCache(this)
	, mEventPipeline()
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



	    // we are done here
	    Logger::info("Stop signal received. Shutting down engine.");
	}



	Root *Root::smSingleton = nullptr;

	Root *Root::getSingleton()
	{
	    if(smSingleton == nullptr)
	    {
	        STONESHIP_EXCEPT(StoneshipException::INVALID_STATE, "Singleton requested when no root was created yet.");
	    }

	    return smSingleton;
	}


}
