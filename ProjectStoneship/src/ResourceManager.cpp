/*
 * ResourceManager.cpp
 *
 *  Created on: Nov 17, 2015
 *      Author: zalasus
 */

#include "ResourceManager.h"

#include "Logger.h"
#include "Root.h"

namespace Stoneship
{

	ResourceManager::ResourceManager()
	{
	}

	void ResourceManager::addResourcePath(const String &path, ResourcePathType type, UID::Ordinal ordinal)
	{
		Logger::info("Added '" + path + "' as resource path with ordinal " + ordinal);
	}

	void ResourceManager::initializeResources()
	{

	}

}

