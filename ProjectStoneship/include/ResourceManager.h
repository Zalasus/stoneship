/*
 * ResourceManager.h
 *
 *  Created on: Nov 17, 2015
 *      Author: zalasus
 */

#ifndef INCLUDE_RESOURCEMANAGER_H_
#define INCLUDE_RESOURCEMANAGER_H_

#include "Types.h"
#include "String.h"
#include "Record.h"

namespace Stoneship
{

	class ResourceManager
	{
	public:

		enum ResourcePathType
		{
			PATH_FILESYSTEM,
			PATH_ZIP_FILE,
			PATH_GZIP_FILE
		};

		enum ResourceType
		{
		    RES_MODEL,
		    RES_MATERIAL,
		    RES_TEXTURE,
		    RES_SOUND,
		    RES_SHADER,
		    RES_LOCALE
		};

		struct ResourcePathExtension
		{
		    ResourcePathExtension(ResourceType pType, const String &pPath) : type(pType), path(pPath) {};

		    ResourceType type;
		    String path;
		};

		ResourceManager();

		void addResourcePath(const String &path, ResourcePathType type, UID::Ordinal ordinal = UID::SELF_REF_ORDINAL);

		/**
		 * @brief Creates index for all newly added paths and creates priority scheme
		 *
		 * This function finalizes the resource declaration process by indexing all paths and associating folders and
		 * files to their position in the priority queue. Call this everytime you added/changed resource paths and wish to
		 * use the manager to access resources!!!
		 */
		void initializeResources();
		
		

	private:

	
	};

}


#endif /* INCLUDE_RESOURCEMANAGER_H_ */
