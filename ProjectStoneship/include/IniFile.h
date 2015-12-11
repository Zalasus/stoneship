/*
 * IniFile.h
 *
 *  Created on: Dec 8, 2015
 *      Author: zalasus
 */

#ifndef INCLUDE_INIFILE_H_
#define INCLUDE_INIFILE_H_

#include <vector>

#include "Types.h"
#include "String.h"

namespace Stoneship
{

    class IniFile
    {
    public:

        struct IniEntry
        {
            String section;
            String key;
            String value;
        };

        IniFile(bool caseSensitive = true);

        void load(const String &path);

        const String &getValue(const String &section, const String &key, const String &def) const;

        int32_t getValueAsInt(const String &section, const String &key, int32_t def) const;

        float getValueAsFloat(const String &section, const String &key, float def) const;

        bool getValueAsBool(const String &section, const String &key, bool def) const;

        const std::vector<IniEntry> &getEntriesInSection(const String &section) const;


    private:

        bool mCaseSensitive;

        std::vector<IniEntry> mEntries;
    };
}



#endif /* INCLUDE_INIFILE_H_ */
