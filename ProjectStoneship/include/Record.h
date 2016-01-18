/*
 * Record.h
 *
 *  Created on: 29.08.2015
 *      Author: Zalasus
 */

#ifndef INCLUDE_RECORD_H_
#define INCLUDE_RECORD_H_

#include <istream>
#include <sstream>
#include <vector>

#include "Types.h"
#include "String.h"

namespace Stoneship
{

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
		inline String toString() const
		{
		    std::ostringstream str;
		    str << std::hex << ordinal << ":" << std::hex << id;
		    return str.str();
		}

		static const Ordinal SELF_REF_ORDINAL = 0xFFFF;
		static const ID NO_ID = 0xFFFFFFFF;
	};

	struct Record
	{
		Record() = delete; //must not instantiate

		typedef uint8_t ModifyType;
		static const ModifyType MODIFY_OVERWRITE = 0;
		static const ModifyType MODIFY_APPEND = 1;

		typedef uint16_t Type;

		static const Type TYPE_GROUP = 0x0;
		static const Type TYPE_OUTDOOR = 0xC0;
		static const Type TYPE_CHUNK_GROUP = 0xC1;
		static const Type TYPE_CHUNK = 0xC2;
		static const Type TYPE_DUNGEON = 0xC5;
		static const Type TYPE_ENTITY = 0xD0;
		static const Type TYPE_MODIFY = 0xFFF0;
		static const Type TYPE_RESERVED = 0xFFFF;

		static const Type TYPE_LOOKUP_ALL = 0xFFFF; //for lookups only!!!

		typedef uint16_t Subtype;
		static const Subtype SUBTYPE_SUBGROUP = 0x0;
		static const Subtype SUBTYPE_WORLD_MODEL = 0x5;
		static const Subtype SUBTYPE_DISPLAY_NAME = 0x6;
		static const Subtype SUBTYPE_DESCRIPTION = 0x7;
		static const Subtype SUBTYPE_TRADING = 0x8;
		static const Subtype SUBTYPE_INVENTORY = 0x9;
		static const Subtype SUBTYPE_ICON = 0xA;
		static const Subtype SUBTYPE_IDENTIFICATION = 0xB;
		static const Subtype SUBTYPE_POSITION = 0x10;
		static const Subtype SUBTYPE_ENTITY = 0xF0;
		static const Subtype SUBTYPE_ENTITY_ITEM = 0xF2;
		static const Subtype SUBTYPE_DATA = 0x100;
		static const Subtype SUBTYPE_TEXT = 0x105;
		static const Subtype SUBTYPE_CONTAINED_ITEM = 0x10F;
		static const Subtype SUBTYPE_EDITOR = 0xFFF0;
		static const Subtype SUBTYPE_MODIFY_METADATA = 0xFFF9;
	};

	struct RecordHeader
	{
	    typedef uint32_t SizeType;
	    typedef uint32_t ChildRecordCountType;
	    typedef uint16_t FlagType;

		Record::Type type;
		SizeType dataSize;

		union
		{
			FlagType flags;
			Record::Type groupType; //only used in group records
		};

		union
		{
			UID::ID id;
			ChildRecordCountType recordCount; //only used in group records
		};

		inline bool isDeleted() {return flags | 0x01;};

		static const uint32_t SIZE_IN_FILE = 12;
	};

	struct SubrecordHeader
	{
	    typedef uint32_t SizeType;

		Record::Subtype type;
		SizeType dataSize;
	};

}

#endif /* INCLUDE_RECORD_H_ */
