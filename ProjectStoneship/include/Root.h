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
#include "MGFManager.h"
#include "ResourceManager.h"
#include "GameCache.h"
#include "EventPipeline.h"

namespace Stoneship
{

	class Root
	{
	public:

		Root(Options &options);
		~Root();

		Options &getOptions();

		MGFManager &getMGFManager();
		ResourceManager &getResourceManager();
		GameCache &getGameCache();
		EventPipeline &getEventPipeline();

		void loadAllMGFs();

		/**
		 * @brief Runs the game. Blocking call; returns when finished
		 */
		void run();

		IWorld *getActiveWorld();


		static Root *getSingleton();


	private:

		Options &mOptions;

		MGFManager mMGFManager;
		ResourceManager mResourceManager;
		GameCache mGameCache;
		EventPipeline mEventPipeline;

		static Root *smSingleton;
	};

}


#endif /* INCLUDE_ROOT_H_ */
