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
#include "Root.h"
#include "SaveGameFile.h"

namespace Stoneship
{

	class MasterGameFile;
	class EntityBase;

	class MasterGameFileManager
	{
	public:

		MasterGameFileManager(Root *root);
		~MasterGameFileManager();

		void loadMGF(const String &filename);
		uint32_t getLoadedMGFCount() const;
		MasterGameFile *getLoadedMGF(UID::Ordinal ordinal);
		MasterGameFile *getLoadedMGF(const String &filename);

		void loadSGF(const String &savename);

		/**
		 * @brief Searches Record in all top groups
		 */
		RecordAccessor getRecordByID(UID id);

		/**
		 * @brief Searches Record only in the top group of given type.
		 */
		RecordAccessor getRecordByTypeID(UID id, Record::Type type);

		/**
		 * @brief Searches for a record with given editor name
		 *
		 * @param name The editor name of the record
		 */
		RecordAccessor getRecordByEditorName(const String &name, Record::Type type);

		void applyModifications(EntityBase *base);

	private:

		Root *mRoot;
		std::vector<MasterGameFile*> mGameFiles;
		MasterGameFile *mCurrentSaveFile;

		uint32_t mLoadedGameFileCount;
	};

}

#endif /* INCLUDE_MASTERGAMEFILEMANAGER_H_ */
