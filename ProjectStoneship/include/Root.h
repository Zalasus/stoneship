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
#include "GUIManager.h"

namespace Stoneship
{

    //TODO: A god class like this one should be optional
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


	private:

		Options &mOptions;
		
		ResourceManager mResourceManager;
		MGFManager mMGFManager;
		GameCache mGameCache;
		EventPipeline mEventPipeline;
		GUIManager mGuiManager;
	};

}


#endif /* INCLUDE_ROOT_H_ */
