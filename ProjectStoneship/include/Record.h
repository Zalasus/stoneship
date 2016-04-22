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
		static const UID NO_UID;
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
		static const Subtype SUBTYPE_WORLD_MODEL_NAME = 0x5;
		static const Subtype SUBTYPE_DISPLAY_NAME = 0x6;
		static const Subtype SUBTYPE_DESCRIPTION = 0x7;
		static const Subtype SUBTYPE_TRADING = 0x8;
		static const Subtype SUBTYPE_INVENTORY = 0xA;
		static const Subtype SUBTYPE_ICON = 0xB;
		static const Subtype SUBTYPE_IDENTIFICATION = 0xC;
		static const Subtype SUBTYPE_COUNT = 0xD;
		static const Subtype SUBTYPE_POSITION = 0x10;
        static const Subtype SUBTYPE_SCALE = 0x11;
		static const Subtype SUBTYPE_ENTITY = 0xF0;
		static const Subtype SUBTYPE_DATA = 0x100;
		static const Subtype SUBTYPE_TEXT = 0x105;
		static const Subtype SUBTYPE_CONTAINER = 0x10E;
		static const Subtype SUBTYPE_CONTAINED_ITEM = 0x10F;
		static const Subtype SUBTYPE_EDITOR = 0xFFF0;
		static const Subtype SUBTYPE_MODIFY_METADATA = 0xFFF9;
	};

	struct RecordHeader
	{
	    typedef uint32_t SizeType;
	    typedef uint16_t FlagType;
	    typedef uint32_t ChildRecordCountType;

		Record::Type type;
		SizeType dataSize;
		Record::Type groupType; // only for group records
		FlagType flags;
		UID::ID id; // not for group records
		ChildRecordCountType recordCount; // only for group records

		inline /*duh*/ uint32_t sizeInFile() const
		{
		    return type == Record::TYPE_GROUP ? SIZE_GROUP_IN_FILE : SIZE_RECORD_IN_FILE;
		}

		bool operator==(const RecordHeader &r) const
        {
		    if(type != r.type || dataSize != r.dataSize || flags != r.flags)
		    {
		        return false;
		    }

		    if(type == Record::TYPE_GROUP)
		    {
		        return groupType == r.groupType && recordCount == r.recordCount;

		    }else
		    {
		        return id == r.id;
		    }
        }

		bool operator!=(const RecordHeader &r) const
        {
            return !(this->operator ==(r));
        }

		static const uint32_t SIZE_RECORD_IN_FILE = sizeof(type) + sizeof(dataSize) + sizeof(flags) + sizeof(id);
		static const uint32_t SIZE_GROUP_IN_FILE = sizeof(type) + sizeof(dataSize) + sizeof(groupType) + sizeof(flags) + sizeof(recordCount);

		static const FlagType FLAG_DELETED = 0x0001;
		static const FlagType FLAG_ID_PRESENT = 0x0002;
		static const FlagType FLAG_EDATA_PRESENT = 0x0004;
		static const FlagType FLAG_ATTACHMENT = 0x0008;
		static const FlagType FLAG_BLOB = 0x0010;
		static const FlagType FLAG_TOP_GROUP = 0x0020;
	};

	struct SubrecordHeader
	{
	    typedef uint32_t SizeType;

		Record::Subtype type;
		SizeType dataSize;

		inline /*duh*/ uint32_t sizeInFile() const
		{
		    return SIZE_IN_FILE;
		}

		bool operator==(const SubrecordHeader &r) const
		{
		    return type == r.type && dataSize == r.dataSize;
		}

		bool operator!=(const SubrecordHeader &r) const
        {
		    return !(this->operator ==(r));
        }

		static const uint32_t SIZE_IN_FILE = sizeof(type) + sizeof(dataSize);
	};

}

#endif /* INCLUDE_RECORD_H_ */
