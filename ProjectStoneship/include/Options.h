/*
 * Options.h
 *
 *  Created on: Nov 9, 2015
 *      Author: zalasus
 */

#ifndef INCLUDE_OPTIONS_H_
#define INCLUDE_OPTIONS_H_

#include "String.h"
#include "IniFile.h"

namespace Stoneship
{

	class Options
	{
	public:

		Options();
		
		void parse(int argc, const char **argv);
		bool isConfigFileLoaded() const;
		void loadConfigFile(const String &filename);

		std::vector<String> &getMGFList();
		
		
    private:
    
		bool mConfigLoaded;
		IniFile mConfig;

        std::vector<String> mMGFList;    

	};

}


#endif /* INCLUDE_OPTIONS_H_ */
