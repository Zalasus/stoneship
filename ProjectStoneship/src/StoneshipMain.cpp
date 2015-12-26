/*
 * StoneshipMain.cpp
 *
 *  Created on: 26.08.2015
 *      Author: Zalasus
 */

#include <iostream>

#include "Stoneship.h"

using namespace Stoneship;

int main(int argc, char **argv)
{
    Stoneship::Logger::getDefaultLogger().setEnableTimestamp(false); //get rid of those annoying timestamps
    Stoneship::Logger::info(Stoneship::String("Initializing Stoneship v") + STONESHIP_VERSION + "...");

	Stoneship::Root *root = new Stoneship::Root();

	root->getOptions().load(argc, argv);

	try
	{
	    root->run();

	}catch(Stoneship::StoneshipException &e)
	{
	    Stoneship::Logger::severe(e.getMessage());
	}

	Stoneship::Logger::info(Stoneship::String("Total tp: getcs=") + Stoneship::MGFDataReader::GETCS + " seeks=" + Stoneship::MGFDataReader::SEEKS + " tells=" + Stoneship::MGFDataReader::TELLS);

	delete root;

	return 0;
}


