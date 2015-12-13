/*
 * ResourceManager.cpp
 *
 *  Created on: Nov 17, 2015
 *      Author: zalasus
 */

#include "ResourceManager.h"

#include <IFileSystem.h>

#include "Logger.h"
#include "Root.h"

namespace Stoneship
{

	ResourceManager::ResourceManager(Root *root)
	: mRoot(root)
	{
	}

	void ResourceManager::addResourcePath(const String &path, UID::Ordinal ordinal, ResourcePathType type, ResourcePathPriority prio)
	{
		if(prio != PRIORITY_DEFAULT)
		{
		    Logger::warn("Resource priority different than default used. This is not implemented at the moment. Setting to default instead.");

		    prio = PRIORITY_DEFAULT;
		}

		Logger::info("Added '" + path + "' as resource path with ordinal " + ordinal);
	}

	void ResourceManager::initializeResources()
	{

	}

}

