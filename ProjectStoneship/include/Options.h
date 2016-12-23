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
		
		std::vector<String> &getMGFList();
		
		
    private:
    
        std::vector<String> mMGFList;    

	};

}


#endif /* INCLUDE_OPTIONS_H_ */
