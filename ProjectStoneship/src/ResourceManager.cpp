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

	ResourceManager::ResourceManager(Root *root)
	: mRoot(root)
	{
	}

	void ResourceManager::addMGFResourcePath(const String &path, UID::Ordinal ordinal, ResourcePathType type, ResourcePathPriority prio)
	{
		Logger::info("Added '" + path + "' as resource path of MGF " + ordinal);
	}

	void ResourceManager::addDefaultResourcePath(const String &path, ResourcePathType type)
    {
        Logger::info("Added '" + path + "' as default resource path");
    }

	void ResourceManager::initializeResources()
	{

	}

	const ResourceManager::ResourcePathExtension ResourceManager::smPathExt[] =
	{
	    {RES_MODEL,    "mdl"},
	    {RES_MATERIAL, "mat"},
	    {RES_TEXTURE,  "tex"},
	    {RES_SOUND,    "snd"},
	    {RES_SHADER,   "shd"},
	    {RES_LOCALE,   "loc"}
	};

}

