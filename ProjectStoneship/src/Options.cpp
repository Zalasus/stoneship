/*
 * Options.cpp
 *
 *  Created on: Nov 18, 2015
 *      Author: zalasus
 */

#include "Options.h"

#include "Root.h"
#include "Logger.h"
#include "MGFManager.h"

namespace Stoneship
{

	Options::Options()
	: mConfigLoaded(false)
	{
	}
	
	void Options::parse(int argc, const char **argv)
	{
	    for(int i = 1; i < argc; ++i)
	    {
	        String currentArg(argv[i]);

	        if(currentArg == "-c")
	        {
	            if(mConfigLoaded)
	            {
	                STONESHIP_EXCEPT(StoneshipException::DATA_FORMAT, "Loading of multiple config files not allowed");
	            }

	            if(i+1 == argc)
	            {
	                STONESHIP_EXCEPT(StoneshipException::DATA_FORMAT, "-c option needs config file argument");
	            }

	            loadConfigFile(String(argv[++i]));

	        }else if(currentArg == "-m")
	        {
	            if(i+1 == argc)
                {
                    STONESHIP_EXCEPT(StoneshipException::DATA_FORMAT, "-m option needs MGF path argument");
                }

                mMGFList.push_back(String(argv[++i]));
	        }
	    }
	}

    bool Options::isConfigFileLoaded() const
    {
        return mConfigLoaded;
    }

    void Options::loadConfigFile(const String &filename)
    {
        mConfig.load(filename);

        std::vector<IniFile::IniEntry> mgfEntries = mConfig.getEntriesInSection("mgf");
        for(auto it = mgfEntries.begin(); it != mgfEntries.end(); ++it)
        {
            mMGFList.push_back(it->value);
        }

        mConfigLoaded = true;
    }

	std::vector<String> &Options::getMGFList()
	{
	    return mMGFList;
	}
}


