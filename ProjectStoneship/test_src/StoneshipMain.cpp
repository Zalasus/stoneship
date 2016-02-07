/*
 * StoneshipMain.cpp
 *
 *  Created on: 26.08.2015
 *      Author: Zalasus
 */

#include <iostream>
#include <fstream>

#include "Stoneship.h"
#include "sas/SASEntityBases.h"

using namespace Stoneship;

int main(int argc, char **argv)
{
    bool gracefully = true;

    Logger::getDefaultLogger().setEnableTimestamp(false); //get rid of those annoying timestamps on the console. let the file logger do this
    Logger::info("Initializing Stoneship v" STONESHIP_VERSION "...");

    std::ofstream logStream("stoneship.log", std::ios::out | std::ios::app);
    if(logStream.good())
    {
        // put an empty line between executions just for readability
        logStream << std::endl;
    }
    Logger fileLogger("Logfile", &logStream);
    Logger::getDefaultLogger().setChildLogger(&fileLogger);

    Root *root = new Stoneship::Root();

    try
	{

        root->getOptions().load(argc, argv);
        root->loadAllMGFs();

        UID uid = root->getMGFManager().getNewUID();
        WorldDungeon dungeon(uid);
        root->getGameCache().manageWorld(&dungeon);
        dungeon.setWorldName("Naughty Zal's sex dungeon");


        root->getMGFManager().storeSGF("save.sgf");

        root->run();


	}catch(StoneshipException &e)
	{
	    Logger::severe(e.getMessage());
	    gracefully = false;
	}

	Logger::info(String("Total tp: getcs=") + MGFDataReader::GETCS + " seeks=" + MGFDataReader::SEEKS + " tells=" + MGFDataReader::TELLS);

	if(gracefully)
	{
	    Logger::info("Stopping gracefully...");

	}else
	{
	    Logger::warn("Stopping with errors...");
	}

	delete root;

	logStream.close();

	return 0;
}


