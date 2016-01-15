/*
 * MasterGameFile.h
 *
 *  Created on: 26.08.2015
 *      Author: Zalasus
 */

#ifndef INCLUDE_MASTERGAMEFILE_H_
#define INCLUDE_MASTERGAMEFILE_H_

#include <vector>
#include <fstream>
#include <ctime>

#include "Types.h"
#include "String.h"
#include "Record.h"
#include "RecordAccessor.h"

namespace Stoneship
{

    class MGFManager;
    class ResourceManager;
    class IEntityBase;
    class IRecordLoadable;

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
			Record::ModifyType modType;
		};

		MasterGameFile(const String &filename, UID::Ordinal ordinal, MGFManager *mgfManager, ResourceManager *resourceManager);
		virtual ~MasterGameFile();

		virtual void load();
		virtual void unload();
		virtual void store();

		const String &getFilename() const;
		UID::Ordinal getOrdinal() const;
		const String &getAuthor() const;
		const String &getDescription() const;
		const std::tm *getTimestamp() const;
		uint16_t getDependencyCount() const;
		const Dependency *getDependencies() const;
		uint16_t getResourceCount() const;
		uint32_t getRecordGroupCount() const;
		bool isLoaded() const;

		/**
		 * @brief Translates an ordinal in this file according to the dependency table (see MGF definition for more information).
		 */
		UID::Ordinal localToGlobalOrdinal(UID::Ordinal local);

		RecordAccessor getRecordByID(UID::ID id);
		RecordAccessor getRecordByTypeID(UID::ID id, Record::Type type);

		RecordAccessor getRecordByEditorName(const String &name, Record::Type type);

		RecordAccessor getFirstRecord(Record::Type type);

		void applyModifications(IRecordLoadable *loadable);

	private:

		inline void _indexModifies(uint32_t recordCount);
		OffsetHint *_getHint(Record::Type t);

		String mFilename;
		std::ifstream mInputStream;
		UID::Ordinal mOrdinal;
		MGFManager *mMGFManager;
		ResourceManager *mResourceManager;

		bool mLoaded;

		uint32_t mFlags;
		String mAuthor;
		String mDescription;
		std::tm mTimestamp;

		uint16_t mDependencyCount;
		Dependency *mDependencies;
		uint16_t mResourceCount;
		OffsetHint *mHints;
		OffsetHint *mCachedHint;
		uint32_t mRecordCount;
		uint32_t mRecordGroupCount;

		std::streampos mHeaderEndOfffset;

		std::vector<ModHint> mMods; //TODO: Replace this with array. we know the total amount of records


		static const uint8_t RES_TYPE_SINGLE = 0;
		static const uint8_t RES_TYPE_FS = 1;
		static const uint8_t RES_TYPE_ZIP = 2;
		static const uint8_t RES_TYPE_GZIP = 3;

		static const uint8_t RES_PRIO_DEFAULT = 0;
		static const uint8_t RES_PRIO_BEFORE = 1;
	};

}

#endif /* INCLUDE_MASTERGAMEFILE_H_ */
