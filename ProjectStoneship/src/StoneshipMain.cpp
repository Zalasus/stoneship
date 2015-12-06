/*
 * StoneshipMain.cpp
 *
 *  Created on: 26.08.2015
 *      Author: Zalasus
 */

#include <Exception.h>
#include <MGFManager.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <ctime>

#include "Stoneship.h"

#include "bprinter/table_printer.h"

static Stoneship::Root *root;
static Stoneship::Player player;

static Stoneship::UID parseUID(const Stoneship::String &in)
{
    std::size_t index = in.find(':');

    if(index != std::string::npos)
    {
        Stoneship::UID::Ordinal ordinal;
        Stoneship::UID::ID id;

        std::istringstream(in.substr(0,index)) >> std::hex >> ordinal;
        std::istringstream(in.substr(index+1,std::string::npos)) >> std::hex >> id;

        return Stoneship::UID(ordinal,id);

    }else
    {
        uint64_t rawUID;

        std::istringstream(in) >> std::hex >> rawUID;

        return Stoneship::UID(rawUID);
    }
}

/*static Stoneship::RecordAccessor getRecordByToken(const Stoneship::String &token)
{

	if(!isHexDigit(token[0]))
	{
		return mgfm.getRecordByEditorName();
	}

}*/

static void inv_use(const std::vector<Stoneship::String> &args)
{
	if(args.size() < 2)
	{
		std::cout << "Usage: inv_use <Inventory Index>" << std::endl << "Uses an item in the player's inventory" << std::endl;
		return;
	}

	uint32_t index;
	std::istringstream(args[1]) >> index;

	if(index >= player.getInventory().getItems().size())
	{
		std::cout << "Invalid inventory index" << std::endl;
		return;
	}

	Stoneship::ItemStack *stack = &(player.getInventory().getItems()[index]);

	if(!stack->getItemBase()->onUse(stack, &player))
	{
		std::cout << "Can't use " << stack->getItemBase()->getDisplayName() << std::endl;
	}
}

static void inv_identify(const std::vector<Stoneship::String> &args)
{
	if(args.size() < 2)
	{
		std::cout << "Usage: inv_identify <Inventory Index>" << std::endl << "Command used to identify items in the player's inventory" << std::endl;
		return;
	}

	uint32_t index;
	std::istringstream(args[1]) >> index;

	if(index >= player.getInventory().getItems().size())
	{
		std::cout << "Invalid inventory index" << std::endl;
		return;
	}

	const Stoneship::ItemStack &stack = player.getInventory().getItems()[index];

	if(!stack.getItemBase()->isUnidentified())
	{
		std::cout << "You already know everything about this item" << std::endl;
		return;
	}

	Stoneship::IEntityBaseItem *identifiedBase = static_cast<Stoneship::IEntityBaseItem*>(root->getEntityManager()->getBase(stack.getItemBase()->getIdentifiedUID()));

	if(identifiedBase == nullptr) // should never happen...
	{
		std::cout << "Identified base does not exist" << std::endl;
		return;
	}

	player.getInventory().removeItem(index, stack.getCount());
	player.getInventory().addItem(identifiedBase, stack.getCount());
	std::cout << "Item identified as '" << identifiedBase->getDisplayName() << "'" << std::endl;
}

static void inv_ls(const std::vector<Stoneship::String> &args)
{
	bprinter::TablePrinter tp(&std::cout);
	tp.AddColumn("Index", 6);
	tp.AddColumn("Name", 40);
	tp.AddColumn("Value", 6);
	tp.AddColumn("Count", 6);

	tp.PrintHeader();

	uint32_t index = 0;
	auto it = player.getInventory().getItems().begin();
	while(it != player.getInventory().getItems().end())
	{
		tp << index << it->getItemBase()->getDisplayName() << it->getItemBase()->getValue() << it->getCount();

		for(uint32_t i = 1; i < it->getItemBase()->getSlots(); ++i)
		{
			tp << "~" << bprinter::endl();
		}

		++index;
		++it;
	}
	tp.PrintFooter();

	std::cout << player.getInventory().getUsedSlots() << "/" << player.getInventory().getSlots() << " Slots used" << std::endl;
}

static void inv_info(const std::vector<Stoneship::String> &args)
{

	if(args.size() < 2)
	{
		std::cout << "Usage: inv_info <Inventory Index>" << std::endl << "Displaysinformation on an item in the player's inventory" << std::endl;
		return;
	}

	uint32_t index;
	std::istringstream(args[1]) >> index;

	if(index >= player.getInventory().getItems().size())
	{
		std::cout << "Invalid inventory index" << std::endl;
		return;
	}

	Stoneship::ItemStack &stack = player.getInventory().getItems()[index];
	Stoneship::IEntityBaseItem * base = stack.getItemBase();

	std::cout << stack.getCount() << " x [" << base->getDisplayName() << "] (" << base->getBaseName() << ")" << std::endl;
	std::cout << base->getDescription() << std::endl;
	std::cout << "Value: " << base->getValue() << std::endl;
	std::cout << "Size:  " << (uint32_t)base->getSlots() << std::endl;

}

static void inv_add(const std::vector<Stoneship::String> &args)
{
	if(args.size() < 2)
	{
		std::cout << "Usage: inv_add <Item UID> [Count] [Type ID]" << std::endl << "Command used to add items to the players inventory" << std::endl;
		return;
	}

	Stoneship::UID uid = parseUID(args[1]);
	uint32_t count = 1;
	if(args.size() >= 3)
	{
		std::istringstream(args[2]) >> count;
	}
	Stoneship::Record::Type type = Stoneship::Record::TYPE_LOOKUP_ALL;
	if(args.size() >= 4)
	{
		std::istringstream(args[2]) >> std::hex >> type;
	}

	try
	{
		Stoneship::IEntityBase *base = root->getEntityManager()->getBase(uid, type);

		if(player.getInventory().addItem(base, count) < count)
		{
			std::cout << "Inventory is full" << std::endl;
		}

	}catch(Stoneship::StoneshipException &e)
	{
		std::cout << "Error:" << e.what() << std::endl;
	}
}

static void world_enter(const std::vector<Stoneship::String> &args)
{
	if(args.size() < 2)
	{
		std::cout << "Usage: enter <Dungeon UID>" << std::endl << "Command used to enter a dungeon/world" << std::endl;
		return;
	}

	Stoneship::UID uid = parseUID(args[1]);

	try
	{
		root->getWorldManager()->enterWorld(uid);

	}catch(Stoneship::StoneshipException &e)
	{
		std::cout << "Error: " << e.what() << std::endl;
	}
}

static void world_look(const std::vector<Stoneship::String> &args)
{

    Stoneship::IWorld *world = root->getWorldManager()->getCurrentWorld();
    if(world != nullptr)
    {

        std::cout << "You are standing in " << world->getWorldName() << std::endl;
        std::cout << "You see: " << std::endl;

        const std::vector<Stoneship::IEntity*> entities = world->getLoadedEntities();
        for(uint32_t i = 0 ; i < entities.size(); ++i)
        {
            if(entities[i]->getEntityType() & Stoneship::IEntity::ENTITYTYPE_WORLD)
            {
                Stoneship::IEntityBaseWorld *wob = static_cast<Stoneship::IEntityBaseWorld*>(entities[i]->getBase());

                std::cout << "[" << i << "] ";

                if(entities[i]->getEntityType() & Stoneship::IEntity::ENTITYTYPE_ITEM)
                {
                    std::cout << static_cast<Stoneship::EntityItem*>(entities[i])->getCount() << " ";

                }else
                {
                    std::cout << "1 ";
                }

                std::cout << wob->getBaseName() << std::endl;
                std::cout << "    Model: " << wob->getModelName() << " (Scaled x" << wob->getModelScale() << ")" << std::endl;

            }else
            {
                std::cout << "A special hidden entity you should actually not see. Please close your eyes." << std::endl;
            }
        }

    }else
    {
        std::cout << "You have not entered a world yet" << std::endl;
    }

}

static void world_interact(const std::vector<Stoneship::String> &args)
{
	uint32_t index;
	std::istringstream(args[1]) >> index;

	Stoneship::IWorld *world = root->getWorldManager()->getCurrentWorld();

	if(world == nullptr)
	{
	    std::cout << "No world loaded" << std::endl;

	    return;
	}

	if(index > world->getLoadedEntityCount())
	{
		std::cout << "Invalid entity index" << std::endl;
		return;
	}

	Stoneship::IEntity *target = world->getLoadedEntities()[index];
	Stoneship::IEntityBase *targetBase = target->getBase();
	if(!(targetBase->getBaseType() & Stoneship::IEntityBase::BASETYPE_WORLD))
	{
		std::cout << "Can't interact. Not a World Object. That's wierd..." << std::endl;
		return;
	}

	if(!static_cast<Stoneship::IEntityBaseWorld*>(targetBase)->onInteract(target, &player))
	{
		std::cout << "Can't interact with that" << std::endl;
	}
}

static void record_info(const std::vector<Stoneship::String> &args)
{
	if(args.size() < 2)
	{
		std::cout << "Usage: record_info <Record UID> [Type ID]" << std::endl << "Prints information on the type and size of a record" << std::endl;
		return;
	}

	Stoneship::UID uid = parseUID(args[1]);

	Stoneship::Record::Type type = Stoneship::Record::TYPE_LOOKUP_ALL;
	if(args.size() >= 3)
	{
		std::istringstream(args[2]) >> std::hex >> type;
	}

	try
	{
		Stoneship::RecordAccessor rec = root->getMGFManager()->getRecordByTypeID(uid, type);
		std::cout << "Record found with size " << rec.getHeader().dataSize << " and type " << rec.getHeader().type << std::endl;

	}catch(Stoneship::StoneshipException &e)
	{
		std::cout << "Error: " << e.what() << std::endl;
	}

}

static void mgf_info(const std::vector<Stoneship::String> &args)
{
	if(args.size() < 2)
	{
		std::cout << "Usage: mgf_info <MGF ordinal>" << std::endl;
		return;
	}

	try
	{
		uint32_t ordinal;

		std::istringstream(args[1]) >> ordinal;

		if(ordinal >= root->getMGFManager()->getLoadedMGFCount())
		{
			std::cout << "Invalid MGF ordinal: " << ordinal << std::endl;
			return;
		}

		Stoneship::MasterGameFile *mgf = root->getMGFManager()->getLoadedMGF(ordinal);

		std::cout << "[" << ordinal << "]" << std::endl;

		std::cout << "Filename:      " << mgf->getFilename() << std::endl;
		std::cout << "Author:        " << mgf->getAuthor() << std::endl;
		std::cout << "Description:   " << mgf->getDescription() << std::endl;
		std::cout << "Created:       " << std::asctime(mgf->getTimestamp()); // CRLF is included

		std::cout << "Depends on:    ";
		for(uint32_t i = 0; i < mgf->getDependencyCount(); i++)
		{
			std::cout << "[" <<  mgf->getDependencies()[i].ordinal << "]" << mgf->getDependencies()[i].filename;

			if(i+1 < mgf->getDependencyCount())
			{
				std::cout << ", ";
			}
		}
		std::cout << std::endl;

		std::cout << "Record groups: " << mgf->getRecordGroupCount() << std::endl;

	}catch(...)
	{
		std::cout << "mgfinfo: Invalid ordinal number" << std::endl;
	}
}

static void mgf_ls(const std::vector<Stoneship::String> &args)
{
	for(uint32_t i = 0; i < root->getMGFManager()->getLoadedMGFCount(); i++)
	{
		std::cout << "[" << i << "] " << root->getMGFManager()->getLoadedMGF(i)->getFilename() << std::endl;
	}

	std::cout << root->getMGFManager()->getLoadedMGFCount() << " MGF(s)" << std::endl;
}

static void manager_info(const std::vector<Stoneship::String> &args)
{
	std::cout << "Loaded MGFs:                " << root->getMGFManager()->getLoadedMGFCount() << std::endl;
	std::cout << "Registered Modify records:  -" << std::endl;
	std::cout << "Cached Entity Bases         " << root->getEntityManager()->getBaseCacheSize() << std::endl;

	if(root->getWorldManager()->getCurrentWorld() != nullptr)
	{
	    std::cout << "Loaded Entities:            " << root->getWorldManager()->getCurrentWorld()->getLoadedEntityCount() << std::endl;

	}else
	{
	    std::cout << "Loaded Entities:             -" << std::endl;
	}
}

static void load(const std::vector<Stoneship::String> &args)
{
	if(args.size() < 2)
	{
		std::cout << "Usage: load <save name>" << std::endl;
		return;
	}

	root->getMGFManager()->loadSGF(args[1]);
}

static void save(const std::vector<Stoneship::String> &args)
{
	if(args.size() < 2)
	{
		std::cout << "Usage: save <save name>" << std::endl;
		return;
	}


}

static void prompt()
{
	bool stopped = false;

	while(!stopped)
	{
		std::cout << ">";

		Stoneship::String line;
		std::getline(std::cin, line);

		std::vector<Stoneship::String> tokens;
		Stoneship::StringUtils::split(line, " ", tokens);

		if(tokens.size() < 1)
		{
			continue;
		}

		if(tokens[0] == "exit")
		{
			stopped = true; //I know a 'break' would do. I just don't like uncontrolled loops

		}else if(tokens[0] == "mgf_info")
		{
			mgf_info(tokens);

		}else if(tokens[0] == "mgf_ls")
		{
			mgf_ls(tokens);

		}else if(tokens[0] == "record_info")
		{
			record_info(tokens);

		}else if(tokens[0] == "world_enter")
		{
			world_enter(tokens);

		}else if(tokens[0] == "world_look")
		{
			world_look(tokens);

		}else if(tokens[0] == "world_interact")
		{
			world_interact(tokens);

		}else if(tokens[0] == "inv_ls")
		{
			inv_ls(tokens);

		}else if(tokens[0] == "inv_add")
		{
			inv_add(tokens);

		}else if(tokens[0] == "inv_info")
		{
			inv_info(tokens);

		}else if(tokens[0] == "inv_identify")
		{
			inv_identify(tokens);

		}else if(tokens[0] == "inv_use")
		{
			inv_use(tokens);

		}else if(tokens[0] == "manager_info")
		{
			manager_info(tokens);

		}else if(tokens[0] == "load")
		{
			load(tokens);

		}else if(tokens[0] == "save")
		{
			save(tokens);

		}else
		{
			std::cout << "Unrecognized command: " << tokens[0] << std::endl;
		}

		std::cout << std::endl;
	}
}


int main(int argc, char **argv)
{

	root = new Stoneship::Root();

	std::cout << std::endl;
	std::cout << "Project Stoneship Utility 1.0" << std::endl;
	std::cout << "-----------------------------" << std::endl << std::endl;


	for(int i = 1; i < argc; ++i)
	{
		std::cout << "Loading '" << argv[i] << "'" << std::endl;

		try
		{

			root->getMGFManager()->loadMGF(Stoneship::String(argv[i]));

		}catch(Stoneship::StoneshipException &e)
		{
			std::cout << "Error while loading: " << e.what() << std::endl;
		}
	}

	std::cout << "Loaded " << root->getMGFManager()->getLoadedMGFCount() << " MGF(s)" << std::endl;

	try
	{

		prompt();

	}catch(Stoneship::StoneshipException &e)
	{
		std::cout << "Error: " << e.what() << std::endl;

	}catch(Stoneship::String &s)
	{
		std::cout << "A whooping great error occured: " << s << std::endl;
	}

	delete root;

	return 0;
}


