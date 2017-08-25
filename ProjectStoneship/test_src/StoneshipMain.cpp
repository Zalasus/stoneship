/*
 * StoneshipMain.cpp
 *
 *  Created on: 26.08.2015
 *      Author: Zalasus
 */

#ifndef _LIBRARY

#include <iostream>
#include <fstream>

#include "Stoneship.h"
#include "sas/SASEntityBases.h"
#include "sas/SASEntities.h"
#include "cxxopts.hpp"

using namespace Stoneship;

int main(int argc, const char **argv)
{
    bool gracefully = true;

    Options options;
    try
    {

        options.parse(argc, argv);
        if(!options.isConfigFileLoaded())
        {
            // load default config
            options.loadConfigFile("stoneship.cfg");
        }

    }catch(StoneshipException &e)
    {
        std::cerr << e.getMessage() << std::endl;
        return 1;
    }

    Logger::getDefaultLogger().setPrintDebug(true);
    Logger::info("Initializing Stoneship v" STONESHIP_VERSION "...");

    std::ofstream logStream("stoneship.log", std::ios::out);
    Logger fileLogger("Logfile", &logStream);
    fileLogger.setEnableTimestamp(true);
    fileLogger.setPrintDebug(true);
    Logger::getDefaultLogger().setChildLogger(&fileLogger);

    Root root(options);
    
    EntityBaseFactory::logRegisteredBases();

    try
	{

        root.getResourceManager().addResourcePath(STONESHIP_DEFAULT_RESOURCE_PATH, ResourceManager::PATH_FILESYSTEM);
        root.loadAllMGFs();

        /*for(uint32_t i = 0; i < 10; ++i)
        {
            UID uid = root.getMGFManager().getNewUID();
            EntityBase_Book *book = new EntityBase_Book(uid);
            root.getGameCache().manageElement(book);
            book->setDisplayName("A book at bedtime");
            book->setText(String("This is the book with ID ") + uid.toString());
            book->setDescription("Test Test Test");
            book->setIconFile("book.png");
            book->setModelName("book.model");
            book->setEditorName("book_id_" + uid.toString());
        }

        EntityBase_Stuff *stuff = new EntityBase_Stuff(root.getMGFManager().getNewUID());
        root.getGameCache().manageElement(stuff);
        stuff->setDisplayName("Gunk");
        stuff->setDescription("Dunkygunkydunkygunk");

        EntityBase_Weapon *weapon = new EntityBase_Weapon(root.getMGFManager().getNewUID());
        root.getGameCache().manageElement(weapon);
        weapon->setDisplayName("Rusty sword");
        weapon->setDescription("A random test weapon");
        weapon->setModelName("weap_sword_rusty.model");

        WorldDungeon *dungeon = new WorldDungeon(0xFFFFDEADBEEF);
        root.getGameCache().manageElement(dungeon);
        dungeon->setWorldName("Random cave");
        for(uint32_t i = 0; i < 5; ++i)
        {
            IEntity *entity = stuff->createEntity(root.getMGFManager().getNewUID());
            entity->setEditorName("ent_stuff_id_" + entity->getCreatedUID().toString());
            dungeon->addEntity(entity);
        }

        EntityBase_Container *containerBase = new EntityBase_Container(root.getMGFManager().getNewUID());
        root.getGameCache().manageElement(containerBase);
        containerBase->setSlotCount(20);
        containerBase->setEditorName("cont_test_1");
        containerBase->setModelName("chest1.model");

        EntityContainer *containerEntity = static_cast<EntityContainer*>(containerBase->createEntity(root.getMGFManager().getNewUID()));
        containerEntity->getInventory().addItem(stuff, 10);
        containerEntity->getInventory().addItem(weapon, 1);
        containerEntity->setEditorName("ent_cont");
        dungeon->addEntity(containerEntity);
        
        WorldOutdoor *outdoor = new WorldOutdoor(0xFFFFCAFEBABE);
        root.getGameCache().manageElement(outdoor);
        outdoor->setWorldName("Plains of Tarquay");
        
        for(int32_t x = -10; x <= 10; ++x)
        {
            for(int32_t y = -10; y <= 10; ++y)
            {
                UID uid = root.getMGFManager().getNewUID();
                Chunk *chunk = new Chunk(uid, outdoor);
                outdoor->getLoadedChunks().push_back(chunk);
                
                chunk->setGridPosition(osg::Vec2i(x,y));
            }
        }

        root.getMGFManager().storeSGF("master.mgf", &root.getGameCache());*/

        root.run();


	}catch(StoneshipException &e)
	{
	    Logger::severe(e.getMessage());
	    gracefully = false;
	}

	Logger::debug(String("Total read tp:  getcs=") + MGFDataReader::GETCS + " seeks=" + MGFDataReader::SEEKS + " tells=" + MGFDataReader::TELLS);
	Logger::debug(String("Total write tp: putcs=") + MGFDataWriter::PUTCS + " seeks=" + MGFDataWriter::SEEKS + " tells=" + MGFDataWriter::TELLS);


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
            out << in.rdbuf();
        }
        out.close();
        in.close();

        return -1;
    }

	return 0;
}

#endif


