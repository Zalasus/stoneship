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
#include "sas/SASEntities.h"

using namespace Stoneship;

#ifndef _LIBRARY

int main(int argc, char **argv)
{
    bool gracefully = true;

    Logger::getDefaultLogger().setEnableTimestamp(false); //get rid of those annoying timestamps on the console. let the file logger do this
    Logger::getDefaultLogger().setPrintDebug(true);
    Logger::info("Initializing Stoneship v" STONESHIP_VERSION "...");

    std::ofstream logStream("stoneship.log", std::ios::out);
    Logger fileLogger("Logfile", &logStream);
    fileLogger.setPrintDebug(true);
    Logger::getDefaultLogger().setChildLogger(&fileLogger);

    Options options();

    Root *root = new Stoneship::Root(options);

    try
	{

        root->getResourceManager().addResourcePath(STONESHIP_DEFAULT_RESOURCE_PATH, ResourceManager::PATH_FILESYSTEM);
        root->loadAllMGFs();

        for(uint32_t i = 0; i < 10; ++i)
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

        EntityBase_Weapon *weapon = new EntityBase_Weapon(root->getMGFManager().getNewUID());
        root->getGameCache().manageBase(weapon);
        weapon->setDisplayName("Rusty sword");
        weapon->setDescription("A random test weapon");

        WorldDungeon *dungeon = new WorldDungeon(root->getMGFManager().getNewUID());
        root->getGameCache().manageWorld(dungeon);
        dungeon->setWorldName("Isao's cave");
        for(uint32_t i = 0; i < 5; ++i)
        {
            IEntity *entity = stuff->createEntity(root->getMGFManager().getNewUID());
            entity->setEditorName("ent_stuff_id_" + entity->getCreatedUID().toString());
            dungeon->addEntity(entity);
        }

        EntityBase_Container *containerBase = new EntityBase_Container(root->getMGFManager().getNewUID());
        root->getGameCache().manageBase(containerBase);
        containerBase->setSlotCount(20);
        containerBase->setEditorName("cont_test_1");
        containerBase->setModelName("chest1.model");

        EntityContainer *containerEntity = static_cast<EntityContainer*>(containerBase->createEntity(root->getMGFManager().getNewUID()));
        containerEntity->getInventory().addItem(stuff, 10);
        containerEntity->getInventory().addItem(weapon, 1);
        containerEntity->setEditorName("ent_cont");
        dungeon->addEntity(containerEntity);

        root->getMGFManager().storeSGF("master.mgf");

        root->run();


	}catch(StoneshipException &e)
	{
	    Logger::severe(e.getMessage());
	    gracefully = false;
	}

	Logger::debug(String("Total read tp:  getcs=") + MGFDataReader::GETCS + " seeks=" + MGFDataReader::SEEKS + " tells=" + MGFDataReader::TELLS);
	Logger::debug(String("Total write tp: putcs=") + MGFDataWriter::PUTCS + " seeks=" + MGFDataWriter::SEEKS + " tells=" + MGFDataWriter::TELLS);


	delete root;

	logStream.close();


	if(gracefully)
    {
        Logger::info("Stopping gracefully...");

        return 0;

    }else
    {
        Logger::warn("Stopping with errors...");
        Logger::warn("Exporting log to stoneship_crash.log");

        std::ofstream out("stoneship_crash.log", std::ios::out);
        std::ifstream in("stoneship.log", std::ios::in);
        if(in.good() && out.good())
        {
            out << in;
        }
        out.close();
        in.close();

        return -1;
    }
}

#endif


