/*
 * MasterGameFileManager.h
 *
 *  Created on: 27.08.2015
 *      Author: Zalasus
 */

#ifndef INCLUDE_MASTERGAMEFILEMANAGER_H_
#define INCLUDE_MASTERGAMEFILEMANAGER_H_

#include <vector>

#include "String.h"
#include "Record.h"
#include "Util.h"

namespace Stoneship
{

	class MasterGameFile;

	class MasterGameFileManager
	{
	public:

		MasterGameFileManager();
		~MasterGameFileManager();

		void loadMGF(const String &filename);
		uint32_t getLoadedMGFCount() const;
		MasterGameFile *getLoadedMGF(UID::Ordinal ordinal);
		MasterGameFile *getLoadedMGF(const String &filename);

		/**
		 * Searches Record in all top groups
		 */
		RecordAccessor getRecord(UID id);

		/**
		 * Searches Record only in the top group of given type.
		 */
		RecordAccessor getRecord(UID id, Record::Type type);


	private:

		std::vector<MasterGameFile*> mGameFiles;

		uint32_t mLoadedGameFileCount;
	};

}

#endif /* INCLUDE_MASTERGAMEFILEMANAGER_H_ */
