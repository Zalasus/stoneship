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
		}

		irr::io::E_FILE_ARCHIVE_TYPE archiveType;
		switch(type)
		{
		case PATH_FILESYSTEM:
		    archiveType = irr::io::EFAT_FOLDER;
		    break;

		case PATH_ZIP_FILE:
		    archiveType = irr::io::EFAT_ZIP;
		    break;

		case PATH_GZIP_FILE:
		    archiveType = irr::io::EFAT_GZIP;
		    break;

		default:
		    STONESHIP_EXCEPT(StoneshipException::RESOURCE_ERROR, "Unknown or unsupported resource type");
		}

		if(!mRoot->getIrrlichtDevice()->getFileSystem()->addFileArchive(path.c_str(), false, true, archiveType))
		{
		    STONESHIP_EXCEPT(StoneshipException::RESOURCE_ERROR, "Could not register resource path " + path);
		}

		Logger::info("Added '" + path + "' as resource path with ordinal " + ordinal);
	}

	void ResourceManager::initializeResources()
	{

	}

}

