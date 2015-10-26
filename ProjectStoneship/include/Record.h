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

		inline uint64_t toUInt64() const { return (static_cast<uint64_t>(ordinal) << 32) | id; };

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

		static const Type TYPE_LOOKUP_ALL = 0xFFFF; //for lookups only!!!

		static const Subtype SUBTYPE_WORLD = 0x2;
		static const Subtype SUBTYPE_ITEM = 0x5;
		static const Subtype SUBTYPE_DATA = 0x100;
		static const Subtype SUBTYPE_EDITOR = 0xFFF0;
	};

	struct RecordHeader
	{
		Record::Type type;
		uint32_t dataSize;
		uint16_t flags;
		UID::ID id;
	};

	struct SubrecordHeader
	{
		Record::Subtype type;
		uint32_t dataSize;
	};

	class RecordAccessor
	{
	public:

		RecordAccessor(const RecordHeader &header, std::istream &stream, MasterGameFile &mgf);

		const RecordHeader &getHeader() const;
		MasterGameFile &getGameFile();

		MGFDataReader &getReader();
		MGFDataReader getReaderForSubrecord(Record::Subtype subtype);

		template <typename T>
		T getSubrecordAs(Record::Subtype subtype);

		RecordAccessor getNextRecord();
		RecordAccessor getChildRecord();

		void rollback();

	private:

		RecordHeader mHeader;
		std::istream &mStream;
		MasterGameFile &mGameFile;
		MGFDataReader mInternalReader;
		std::streampos mOffset; //offset of data field

	};
}

#endif /* INCLUDE_RECORD_H_ */
