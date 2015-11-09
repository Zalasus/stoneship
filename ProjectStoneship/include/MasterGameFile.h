/*
 * MasterGameFile.h
 *
 *  Created on: 26.08.2015
 *      Author: Zalasus
 */

#ifndef INCLUDE_MASTERGAMEFILE_H_
#define INCLUDE_MASTERGAMEFILE_H_

#include <fstream>
#include <ios>
#include <vector>

#include "String.h"
#include "Record.h"
#include "Util.h"


namespace Stoneship
{

	class MasterGameFileManager;
	class EntityBase;

	class MasterGameFile
	{
	public:

		friend class MGFDataReader;
		friend class RecordAccessor;

		struct Dependency
		{
			String filename;
			UID::Ordinal ordinal;
		};

		struct OffsetHint
		{
			Record::Type type;
			std::streampos offset;
			uint32_t recordCount;
		};

		struct ModHint
		{
			UID uid;
			Record::Type type;
			std::streampos offset;
		};

		MasterGameFile(const String &filename, UID::Ordinal ordinal, MasterGameFileManager &manager);
		virtual ~MasterGameFile();

		virtual void load();
		virtual void unload();
		virtual void store();

		const String &getFilename() const;
		UID::Ordinal getOrdinal() const;
		const String &getAuthor() const;
		uint16_t getDependencyCount() const;
		const Dependency *getDependencies() const;
		const String &getDescription() const;
		uint32_t getRecordGroupCount() const;
		uint16_t getResourceCount() const;
		bool isLoaded() const;

		/**
		 * @brief Translates an ordinal in this file according to the dependency table (see MGF definition for more information).
		 */
		UID::Ordinal localToGlobalOrdinal(UID::Ordinal local);

		/**
		 * @brief Creates accessor for a record at the current stream position
		 */
		virtual RecordAccessor getRecord();

		virtual RecordAccessor getRecordByID(UID::ID id);
		virtual RecordAccessor getRecordByTypeID(UID::ID id, Record::Type type);

		virtual RecordAccessor getRecordByEditorName(const String &name, Record::Type type);

		virtual RecordAccessor getFirstRecord(Record::Type type);

		virtual void applyModifications(EntityBase *base);

	private:

		inline void _indexModifies(uint32_t recordCount);
		OffsetHint *_getHint(Record::Type t);


		String mFilename;
		std::ifstream mInputStream;
		UID::Ordinal mOrdinal;
		MasterGameFileManager &mManager;

		bool mLoaded;

		uint32_t mFlags;
		String mAuthor;
		String mDescription;

		uint16_t mDependencyCount;
		Dependency *mDependencies;
		uint16_t mResourceCount;
		OffsetHint *mHints;
		uint32_t mRecordCount;
		uint32_t mRecordGroupCount;

		std::streampos mHeaderEndOfffset;

		std::vector<ModHint> mMods; //TODO: Replace this with array. we know the total amount of records
	};

}

#endif /* INCLUDE_MASTERGAMEFILE_H_ */
