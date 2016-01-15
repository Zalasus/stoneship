/*
 * StoneshipMain.cpp
 *
 *  Created on: 26.08.2015
 *      Author: Zalasus
 */

#include <iostream>

#include "Stoneship.h"
#include "sas/SASEntityBases.h"

using namespace Stoneship;

int main(int argc, char **argv)
{
    Stoneship::Logger::getDefaultLogger().setEnableTimestamp(false); //get rid of those annoying timestamps
    Stoneship::Logger::info(Stoneship::String("Initializing Stoneship v") + STONESHIP_VERSION + "...");

	Stoneship::Root *root = new Stoneship::Root();

	root->getOptions().load(argc, argv);

	Stoneship::UID newUID = root->getMGFManager()->getNewUID();
	Stoneship::EntityBase_Book *book = new EntityBase_Book(newUID);
	root->getEntityManager()->manageBase(book);
	book->setText("This book was created during gameplay. Wooo!");

	try
	{

	    IEntityBase *base = root->getEntityManager()->getBase(newUID);
	    if(base == nullptr || !(base->getBaseType() & Stoneship::IEntityBase::BASETYPE_ITEM))
	    {
	        STONESHIP_EXCEPT(Stoneship::StoneshipException::ENTITY_ERROR, "Entity not found or wrong type of entity");
	    }

	    static_cast<Stoneship::IEntityBaseItem*>(base)->onUse(nullptr, nullptr);

	    root->run();

	}catch(Stoneship::StoneshipException &e)
	{
	    Stoneship::Logger::severe(e.getMessage());
	}

	Stoneship::Logger::info(Stoneship::String("Total tp: getcs=") + Stoneship::MGFDataReader::GETCS + " seeks=" + Stoneship::MGFDataReader::SEEKS + " tells=" + Stoneship::MGFDataReader::TELLS);

	delete root;

	return 0;
}


