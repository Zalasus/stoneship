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

#include "String.h"
#include "Record.h"
#include "Util.h"


namespace Stoneship
{

	class MasterGameFileManager;

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
		};

		MasterGameFile(const String &filename, UID::Ordinal ordinal, MasterGameFileManager &manager);
		~MasterGameFile();

		void load();

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
		 * Translates an ordinal in this file according to the dependency table (see MGF definition for more information).
		 */
		UID::Ordinal localToGlobalOrdinal(UID::Ordinal local);

		RecordAccessor getRecord(UID::ID id);
		RecordAccessor getRecord(UID::ID id, Record::Type type);


	private:

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
		const OffsetHint *mCachedHint;

		std::streampos mHeaderEndOfffset;
	};

}

#endif /* INCLUDE_MASTERGAMEFILE_H_ */
