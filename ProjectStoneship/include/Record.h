/*
 * Record.h
 *
 *  Created on: 29.08.2015
 *      Author: Zalasus
 */

#ifndef INCLUDE_RECORD_H_
#define INCLUDE_RECORD_H_

#include <istream>
#include <vector>

#include "Util.h"
#include "MGFDataReader.h"

namespace Stoneship
{

	class MasterGameFile;

	struct UID
	{
		typedef uint16_t Ordinal;
		typedef uint32_t ID;

		UID();
		UID(Ordinal pOrdinal, ID pId);
		UID(uint64_t l);

		Ordinal ordinal;
		ID id;

		bool operator==(const UID &right);

		inline uint64_t toUInt64() const { return (static_cast<uint64_t>(ordinal) << 32) | id;};

		static const Ordinal SELF_REF_ORDINAL = 0xFFFF;
	};

	struct Record
	{
		Record() = delete; //must not instantiate

		typedef uint16_t Type;
		typedef uint16_t Subtype;

		static const Type TYPE_GROUP = 0x0;
		static const Type TYPE_DUNGEON = 0xC5;
		static const Type TYPE_ENTITY = 0xD0;
		static const Type TYPE_MODIFY = 0xFFF0;

		static const Type TYPE_LOOKUP_ALL = 0xFFFF; //for lookups only!!!

		static const Subtype SUBTYPE_WORLD_MODEL = 0x2;
		static const Subtype SUBTYPE_DISPLAY_NAME = 0x6;
		static const Subtype SUBTYPE_DESCRIPTION = 0x7;
		static const Subtype SUBTYPE_TRADING = 0x8;
		static const Subtype SUBTYPE_INVENTORY = 0x9;
		static const Subtype SUBTYPE_ICON = 0xA;
		static const Subtype SUBTYPE_IDENTIFICATION = 0xB;
		static const Subtype SUBTYPE_DATA = 0x100;
		static const Subtype SUBTYPE_EDITOR = 0xFFF0;
		static const Subtype SUBTYPE_MODIFY_METADATA = 0xFFF9;
	};

	struct RecordHeader
	{
		Record::Type type;
		uint32_t dataSize;

		union
		{
			uint16_t flags;
			Record::Type groupType; //only used in group records
		};

		union
		{
			UID::ID id;
			uint32_t recordCount; //only used in group records
		};

		inline bool isDeleted() {return flags | 0x01;};

		static const uint32_t SIZE_IN_FILE = 12;
	};

	struct SubrecordHeader
	{
		Record::Subtype type;
		uint32_t dataSize;
	};

	class RecordAccessor
	{
	public:

		RecordAccessor(const RecordHeader &header, std::istream *stream, MasterGameFile *mgf);
		RecordAccessor(const RecordAccessor &a);

		const RecordHeader &getHeader() const;
		MasterGameFile *getGameFile();
		std::streampos getDataOffset(); /** @returns Offset of record's data field */
		std::streampos getOffset(); /** @returns Absolute offset of record */


		MGFDataReader &getReader();
		MGFDataReader getReaderForSubrecord(Record::Subtype subtype);

		/**
		 * @brief Creates an Accessor for the next record in the stream.
		 *
		 * Once this method was called, the accessor gets unusable for subrecord access unless rollback() is called. This method can be called
		 * in any stream position as long as it isn't EOF.
		 */
		RecordAccessor getNextRecord();

		/**
		 * @brief Creates accessor for the first record in this record's data field.
		 *
		 * This method is insensitive to stream position and may be called
		 */
		RecordAccessor getFirstChildRecord();

		/**
		 * @brief Creates accessor for the next record in this record's data field.
		 *
		 * Should only be used for Group records and in stream positions right before the next record's header.
		 *
		 * @deprecated Since this method ruins everything when called in a position not between two records, it should not be used.
		 * The recommended way to access successive child records is to use the getFirstChildRecord() method and subsequently calling
		 * the getNextRecord() of the returned RecordAccessor objects since both methods are insensitive to stream position.
		 */
		RecordAccessor getNextChildRecord();

		/**
		 * @brief Moves the stream pointer to the beginning of this records data field.
		 */
		void rollback();

		/**
		 * @brief Moves the stream pointer to the end of this record (the first byte after the data field)
		 */
		void skip();

	private:

		RecordHeader mHeader;
		std::istream *mStream;
		MasterGameFile *mGameFile;
		MGFDataReader mInternalReader;
		std::streampos mDataOffset; //offset of data field

	};
}

#endif /* INCLUDE_RECORD_H_ */
