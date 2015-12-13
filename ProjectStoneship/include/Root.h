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

    class MGFManager;
    class EntityManager;
    class WorldManager;
    class ResourceManager;
    class EventPipeline;

	class Root
	{
	public:

		Root();
		~Root();

		Options &getOptions();

		MGFManager *getMGFManager();
		EntityManager *getEntityManager();
		WorldManager *getWorldManager();
		ResourceManager *getResourceManager();
		EventPipeline *getEventPipeline();

		/**
		 * @brief Runs the game. Blocking call; returns when finished
		 */
		void run();

		static Root *getSingleton();


	private:

		Options mOptions;

		MGFManager *mMGFManager;
		EntityManager *mEntityManager;
		WorldManager *mWorldManager;
		ResourceManager *mResourceManager;
		EventPipeline *mEventPipeline;

		static Root *smSingleton;
	};

}


#endif /* INCLUDE_ROOT_H_ */
