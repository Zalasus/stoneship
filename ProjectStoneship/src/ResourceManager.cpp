/*
 * ResourceManager.cpp
 *
 *  Created on: Nov 17, 2015
 *      Author: zalasus
 */

#include "ResourceManager.h"

#include "Logger.h"

namespace Stoneship
{

	ResourceManager::ResourceManager(Root *root)
	: mRoot(root)
	{
	}

	void ResourceManager::addResourcePath(const String &path, ResourcePathType type)
	{
		Logger::info("Added '" + path + "' as resource path");
	}

}

