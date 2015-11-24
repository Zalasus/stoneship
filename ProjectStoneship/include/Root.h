/*
 * Root.h
 *
 *  Created on: Nov 9, 2015
 *      Author: zalasus
 */

#ifndef INCLUDE_ROOT_H_
#define INCLUDE_ROOT_H_

#include "Types.h"
#include "Options.h"

namespace Stoneship
{

	//TODO: Important methods in Root and all Managers should be virtual to enable creation of custom manager classes by extending existing ones

    class MGFManager;
    class EntityManager;
    class WorldManager;
    class ResourceManager;

	class Root
	{
	public:

		Root();
		virtual ~Root();

		virtual Options &getOptions();

		virtual MGFManager *getMGFManager();
		virtual EntityManager *getEntityManager();
		virtual WorldManager *getWorldManager();
		virtual ResourceManager *getResourceManager();


	private:
		Options mOptions;

		MGFManager *mMGFManager;
		EntityManager *mEntityManager;
		WorldManager *mWorldManager;
		ResourceManager *mResourceManager;

	};

}


#endif /* INCLUDE_ROOT_H_ */
