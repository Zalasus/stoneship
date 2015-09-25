/*
 * StoneshipMain.cpp
 *
 *  Created on: 26.08.2015
 *      Author: Zalasus
 */

#include <iostream>
#include <vector>
#include <sstream>

#include "String.h"
#include "MasterGameFile.h"
#include "MasterGameFileManager.h"
#include "EntityManager.h"
#include "StoneshipException.h"
#include "Inventory.h"

#include "bprinter/table_printer.h"


static Stoneship::MasterGameFileManager mgfm;
static Stoneship::EntityManager entm(mgfm);
static Stoneship::Inventory inv(20);

static Stoneship::UID parseUID(const Stoneship::String &in)
{
	uint64_t rawUID;

	std::istringstream(in) >> std::hex >> rawUID;

	return Stoneship::UID(rawUID);
}

static void inv_use(const std::vector<Stoneship::String> &args)
{
	if(args.size() < 2)
	{
		std::cout << "Usage: inv_use <Inventory Index>" << std::endl << "Uses an item in the player's inventory" << std::endl;
		return;
	}

	uint32_t index;
	std::istringstream(args[1]) >> index;

	if(index >= inv.getItems().size())
	{
		std::cout << "Invalid inventory index" << std::endl;
		return;
	}

	Stoneship::ItemStack &stack = inv.getItems()[index];

	if(!stack.itemBase->onUse(stack))
	{
		std::cout << "Can't use " << stack.itemBase->getDisplayName() << std::endl;
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

	if(index >= inv.getItems().size())
	{
		std::cout << "Invalid inventory index" << std::endl;
		return;
	}

	const Stoneship::ItemStack &stack = inv.getItems()[index];

	if(!stack.itemBase->isUnidentified())
	{
		std::cout << "You already know everything about this item" << std::endl;
		return;
	}

	Stoneship::ItemBase *identifiedBase = static_cast<Stoneship::ItemBase*>(entm.getBase(stack.itemBase->getIdentifiedUID()));

	if(identifiedBase == nullptr)
	{
		std::cout << "Identified base does not exist" << std::endl;
		return;
	}

	inv.removeItem(index, stack.count);
	inv.addItem(identifiedBase, stack.count);
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
	auto it = inv.getItems().begin();
	while(it != inv.getItems().end())
	{
		tp << index << it->itemBase->getDisplayName() << it->itemBase->getValue() << it->count;

		for(uint32_t i = 1; i < it->itemBase->getSlots(); ++i)
		{
			tp << "~" << bprinter::endl();
		}

		++index;
		++it;
	}
	tp.PrintFooter();

	std::cout << inv.getUsedSlots() << "/" << inv.getSlots() << " Slots used" << std::endl;
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
		Stoneship::EntityBase *base = entm.getBase(uid, type);

		if(!inv.addItem(static_cast<Stoneship::ItemBase*>(base), count)) //TODO: make this cast safer without using RTTI
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
		wrldm.enterWorld(uid);

	}catch(Stoneship::StoneshipException &e)
	{
		std::cout << "Error: " << e.what() << std::endl;
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
		Stoneship::RecordAccessor rec = mgfm.getRecord(uid, type);
		std::cout << "Record found with size " << rec.getReader().bytesRemainingInUnit() << std::endl;

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

		if(ordinal >= mgfm.getLoadedMGFCount())
		{
			std::cout << "Invalid MGF ordinal: " << ordinal << std::endl;
			return;
		}

		Stoneship::MasterGameFile *mgf = mgfm.getLoadedMGF(ordinal);

		std::cout << "[" << ordinal << "]" << std::endl;

		std::cout << "Filename:      " << mgf->getFilename() << std::endl;
		std::cout << "Author:        " << mgf->getAuthor() << std::endl;
		std::cout << "Description:   " << mgf->getDescription() << std::endl;

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
		std::cout << "Records:       " << mgf->getRecordCount() << std::endl;

	}catch(...)
	{
		std::cout << "mgfinfo: Invalid ordinal number" << std::endl;
	}
}

static void mgf_ls(const std::vector<Stoneship::String> &args)
{
	for(uint32_t i = 0; i < mgfm.getLoadedMGFCount(); i++)
	{
		std::cout << "[" << i << "] " << mgfm.getLoadedMGF(i)->getFilename() << std::endl;
	}

	std::cout << mgfm.getLoadedMGFCount() << " MGF(s)" << std::endl;
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

		}else if(tokens[0] == "inv_ls")
		{
			inv_ls(tokens);

		}else if(tokens[0] == "inv_add")
		{
			inv_add(tokens);

		}else if(tokens[0] == "inv_identify")
		{
			inv_identify(tokens);

		}else if(tokens[0] == "inv_use")
		{
			inv_use(tokens);

		}else
		{
			std::cout << "Unrecognized command: " << tokens[0] << std::endl;
		}

		std::cout << std::endl;
	}
}


int main(int argc, char **argv)
{
	if(argc < 2)
	{
		std::cout << "Usage:  stoneship <MGF file>*" << std::endl;

		return 1;
	}

	std::cout << std::endl;
	std::cout << "Project Stoneship Utility 1.0" << std::endl;
	std::cout << "-----------------------------" << std::endl << std::endl;

	for(int32_t i = 1; i < argc; i++)
	{
		Stoneship::String path(argv[i]);

		std::cout << "Loading MGF: " << path << std::endl;

		try
		{

			mgfm.loadMGF(path);

		}catch(Stoneship::StoneshipException &e)
		{
			std::cout << "Error while loading: " << e.what() << std::endl;
		}
	}

	std::cout << "Loaded " << mgfm.getLoadedMGFCount() << " MGF(s)" << std::endl;

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

	return 0;
}


