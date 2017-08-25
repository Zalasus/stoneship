/*
 * IniFile.cpp
 *
 *  Created on: Dec 8, 2015
 *      Author: zalasus
 */

#include "IniFile.h"

#include <fstream>

#include "Exception.h"

namespace Stoneship
{

    IniFile::IniFile(bool caseSensitive)
    : mCaseSensitive(caseSensitive)
    {
    }

    void IniFile::load(const String &path)
    {
        // Incredibly primitve parsing method. Makes you look like a Java programmer again >_<
        std::ifstream in(path, std::ios::in);

        if(!in.good())
        {
            STONESHIP_EXCEPT(StoneshipException::IO_ERROR, "Could not open ini file");
        }

        String currentSection = "";

        String line;
        while(std::getline(in, line))
        {
            line = StringUtils::trim(line);

            if(line.empty() || line[0] == '#')
            {
                continue;
            }

            if(line[0] == '[')
            {
                currentSection = StringUtils::trim(line.substr(1, line.length() - 2));

                continue;
            }

            std::size_t index = line.find('=');

            IniEntry entry;
            entry.section = currentSection;

            if(index != String::npos)
            {
                entry.key = StringUtils::trim(line.substr(0,index));
                entry.value = StringUtils::trim(line.substr(index + 1, String::npos));

            }else
            {
                entry.key = "";
                entry.value = StringUtils::trim(line);
            }

            mEntries.push_back(entry);
        }

        in.close();
    }

    const String &IniFile::getValue(const String &section, const String &key, const String &def) const
    {
        for(uint32_t i = 0; i < mEntries.size(); ++i)
        {
            if(mEntries[i].section == section && mEntries[i].key == key)
            {
                return mEntries[i].value;
            }
        }

        return def;
    }

    int32_t IniFile::getValueAsInt(const String &section, const String &key, int32_t def) const
    {
        for(uint32_t i = 0; i < mEntries.size(); ++i)
        {
            if(mEntries[i].section == section && mEntries[i].key == key)
            {
                int32_t v;
                if(StringUtils::parseInt(mEntries[i].value, v))
                {
                    return v;

                }else
                {
                    break;
                }
            }
        }

        return def;
    }

    float IniFile::getValueAsFloat(const String &section, const String &key, float def) const
    {
        for(uint32_t i = 0; i < mEntries.size(); ++i)
        {
            if(mEntries[i].section == section && mEntries[i].key == key)
            {
                float v;
                if(StringUtils::parseFloat(mEntries[i].value, v))
                {
                    return v;

                }else
                {
                    break;
                }
            }
        }

        return def;
    }

    bool IniFile::getValueAsBool(const String &section, const String &key, bool def) const
    {
        for(uint32_t i = 0; i < mEntries.size(); ++i)
        {
            if(mEntries[i].section == section && mEntries[i].key == key)
            {
                const String &v = StringUtils::toLower(mEntries[i].value);

                if(v == "true" || v == "1" || v == "yes")
                {
                    return true;

                }else if(v == "false" || v == "0" || v == "no")
                {
                    return false;

                }else
                {
                    break;
                }
            }
        }

        return def;
    }

    std::vector<IniFile::IniEntry> IniFile::getEntriesInSection(const String &section) const
    {
        std::vector<IniEntry> entries;

        for(uint32_t i = 0; i < mEntries.size(); ++i)
        {
            if(mEntries[i].section == section)
            {
                entries.push_back(mEntries[i]);
            }
        }

        return entries;
    }

}


