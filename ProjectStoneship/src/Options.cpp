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

    const String Options::DEFAULT_INI_FILE_NAME = "stoneship.cfg";

	Options::Options(Root *root)
	: mRoot(root)
	{
	}

	void Options::load(int argc, char **argv)
	{
	    String cfgFile = DEFAULT_INI_FILE_NAME;
	    try
	    {
	        mIniFile.load(cfgFile);

	    }catch(StoneshipException &e)
	    {
	        if(e.getType() != StoneshipException::IO_ERROR)
	        {
	            throw;
	        }

	        Logger::warn("Could not load config file '" + cfgFile + "'. Using default values instead.");
	    }
	}

	const IniFile &Options::getIniFile() const
	{
	    return mIniFile;
	}

	uint32_t Options::getResolutionX() const
	{
	    int32_t x = mIniFile.getValueAsInt("graphics", "resolutionX", 640);
	    if(x <= 0)
	    {
	        x = 640;
	    }

	    return x;
	}

    uint32_t Options::getResolutionY() const
    {
        int32_t x = mIniFile.getValueAsInt("graphics", "resolutionY", 480);
        if(x <= 0)
        {
            x = 480;
        }

        return x;
    }

    bool Options::getLogFPS() const
    {
        return false;
    }

    bool Options::getFullscreen() const
    {
        return mIniFile.getValueAsBool("graphics", "fullscreen", false);
    }

    bool Options::getVsync() const
    {
        return mIniFile.getValueAsBool("graphics", "vsync", false);
    }
}


