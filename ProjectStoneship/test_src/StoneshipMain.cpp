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

#ifndef _LIBRARY

int main(int argc, char **argv)
{
    bool gracefully = true;

    Logger::getDefaultLogger().setEnableTimestamp(false); //get rid of those annoying timestamps on the console. let the file logger do this
    Logger::getDefaultLogger().setPrintDebug(true);
    Logger::info("Initializing Stoneship v" STONESHIP_VERSION "...");

    std::ofstream logStream("stoneship.log", std::ios::out | std::ios::app);
    if(logStream.good())
    {
        // put an empty line between executions for readability
        logStream << std::endl;
    }
    Logger fileLogger("Logfile", &logStream);
    fileLogger.setPrintDebug(true);
    Logger::getDefaultLogger().setChildLogger(&fileLogger);

    Root *root = new Stoneship::Root();

    try
	{

        root->getOptions().load(argc, argv);
        root->getResourceManager().addResourcePath(STONESHIP_DEFAULT_RESOURCE_PATH, ResourceManager::PATH_FILESYSTEM);
        root->loadAllMGFs();

        for(uint32_t i = 0; i < 100; ++i)
        {
            UID uid = root->getMGFManager().getNewUID();
            EntityBase_Book *book = new EntityBase_Book(uid);
            root->getGameCache().manageBase(book);
            book->setDisplayName("A book at bedtime");
            book->setText(String("This is the book with ID ") + uid.toString());
            book->setDescription("Test Test Test");
            book->setIconFile("book.png");
            book->setModelName("book.model");
            book->setEditorName("book_id_" + uid.toString());
        }

        EntityBase_Stuff *stuff = new EntityBase_Stuff(root->getMGFManager().getNewUID());
        root->getGameCache().manageBase(stuff);
        stuff->setDisplayName("Gunk");
        stuff->setDescription("Dunkygunkydunkygunk");

        WorldDungeon *dungeon = new WorldDungeon(root->getMGFManager().getNewUID());
        root->getGameCache().manageWorld(dungeon);
        dungeon->setWorldName("Isao's cave");
        for(uint32_t i = 0; i < 20; ++i)
        {
            dungeon->addEntity(stuff->createEntity(root->getMGFManager().getNewUID()));
        }

        root->getMGFManager().storeSGF("master.mgf");

        root->run();


	}catch(StoneshipException &e)
	{
	    Logger::severe(e.getMessage());
	    gracefully = false;
	}

	Logger::debug(String("Total read tp:  getcs=") + MGFDataReader::GETCS + " seeks=" + MGFDataReader::SEEKS + " tells=" + MGFDataReader::TELLS);
	Logger::debug(String("Total write tp: putcs=") + MGFDataWriter::PUTCS + " seeks=" + MGFDataWriter::SEEKS + " tells=" + MGFDataWriter::TELLS);

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

#endif


