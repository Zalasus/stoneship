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
        root->getResourceManager().addResourcePath(STONESHIP_DEFAULT_RESOURCE_PATH, ResourceManager::PATH_FILESYSTEM);
        root->loadAllMGFs();

        root->getMGFManager().loadSGF("save2.sgf");

        /*UID uid = root->getMGFManager().getNewUID();
        WorldDungeon dungeon(uid);
        root->getGameCache().manageWorld(&dungeon);
        dungeon.setWorldName("Naughty Zal's sex dungeon");

        uid = root->getMGFManager().getNewUID();
        EntityBase_Book *book = new EntityBase_Book(uid);
        root->getGameCache().manageBase(book);
        book->setDisplayName("Book that is to be modified");
        book->setText("If you can read this, the automatic mod saving didn't quite do the trick.");
        book->setModelName("book.model");

        uid = root->getMGFManager().getNewUID();
        book = new EntityBase_Book(uid);
        root->getGameCache().manageBase(book);
        book->setDisplayName("Random book");
        book->setText("Seriously, I have no idea what to put in here.");
        book->setModelName("book.model");

        uid = root->getMGFManager().getNewUID();
        IEntity *entity = book->createEntity(uid);
        dungeon.addEntity(entity);*/

        IEntityBase *base = root->getGameCache().getBase(UID(0,0x0F000001), 0x820);
        EntityBase_Book *book = static_cast<EntityBase_Book*>(base);

        /*book->setDescription("Also, the description changed!");
        book->setText("Modified by MODIFY record. Weeee :3\n"
                      "\n"
                      "Also, have some whitespace\n"
                      "\n");

        root->getMGFManager().storeSGF("save2.sgf");*/

        std::cout << "[" << book->getDisplayName() << "]" << std::endl;
        std::cout << book->getDescription() << std::endl;
        std::cout << "And on the terminal these words appear: " << book->getText() << std::endl;

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


