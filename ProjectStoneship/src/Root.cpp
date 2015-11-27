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
	        Logger::warn("Created second instance of Root object. This indicates an error in the engine");

	    }else
	    {
	        smSingleton = this; //catch any instantiations not mady by the singleton method
	    }
	}

	Root::~Root()
	{
		delete mMGFManager;
		delete mEntityManager;
		delete mWorldManager;
		delete mResourceManager;
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
