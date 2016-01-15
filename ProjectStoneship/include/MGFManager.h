/*
 * MGFManager.h
 *
 *  Created on: 27.08.2015
 *      Author: Zalasus
 */

#ifndef INCLUDE_MGFMANAGER_H_
#define INCLUDE_MGFMANAGER_H_

#include <vector>

#include "Types.h"
#include "String.h"
#include "Record.h"
#include "RecordAccessor.h"


namespace Stoneship
{

    class Root;
    class IEntityBase;
    class MasterGameFile;
    class IRecordLoadable;

	class MGFManager
	{
	public:

		MGFManager(Root *root);
		~MGFManager();

		void loadMGF(const String &filename);
		uint32_t getLoadedMGFCount() const;
		MasterGameFile *getLoadedMGF(UID::Ordinal ordinal);
		MasterGameFile *getLoadedMGF(const String &filename);

		UID getNewUID(UID::Ordinal ordinal = UID::SELF_REF_ORDINAL);

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

		void applyModifications(IRecordLoadable *loadable);

	private:

		Root *mRoot;
		std::vector<MasterGameFile*> mGameFiles;
		MasterGameFile *mCurrentSaveFile;

		uint32_t mLoadedGameFileCount;
	};

}

#endif /* INCLUDE_MGFMANAGER_H_ */
