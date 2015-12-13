/*
 * StoneshipMain.cpp
 *
 *  Created on: 26.08.2015
 *      Author: Zalasus
 */

#include <iostream>

#include "Stoneship.h"


int main(int argc, char **argv)
{
    Stoneship::Logger::info(Stoneship::String("Initializing Stoneship v") + STONESHIP_VERSION + "...");

	Stoneship::Root *root = new Stoneship::Root();

	root->getOptions().load(argc, argv);

	try
	{
	    root->run();

	}catch(Stoneship::StoneshipException &e)
	{
	    Stoneship::Logger::log(e.getMessage(), Stoneship::Logger::LOGLEVEL_SEVERE);
	}

	delete root;

	return 0;
}


