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

namespace Stoneship
{

    class Root;

	class ResourceManager
	{
	public:

		enum ResourcePathType
		{
			FILESYSTEM,
			ZIP_FILE,
			GZIP_FILE
		};

		ResourceManager(Root *root);

		void addResourcePath(const String &path, ResourcePathType type);


	private:

		Root *mRoot;

	};

}


#endif /* INCLUDE_RESOURCEMANAGER_H_ */
