/*
 * SaveGameFile.h
 *
 *  Created on: Nov 6, 2015
 *      Author: zalasus
 */

#ifndef INCLUDE_SAVEGAMEFILE_H_
#define INCLUDE_SAVEGAMEFILE_H_

#include "MasterGameFile.h"

namespace Stoneship
{

	class SaveGameFile : public MasterGameFile
	{
	public:

		SaveGameFile(const String &filename, UID::Ordinal ordinal, MGFManager *manager, ResourceManager *resourceManager);
		~SaveGameFile();

		void load();
		void store();

	private:



	};

}

#endif /* INCLUDE_SAVEGAMEFILE_H_ */
