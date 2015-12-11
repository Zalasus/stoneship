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

	class Root;

	class Options
	{
	public:

		Options(Root *root);

		void load(int argc, char **argv);

		const IniFile &getIniFile() const;

		uint32_t getResolutionX() const;
		uint32_t getResolutionY() const;
		bool getFullscreen() const;
		bool getVsync() const;
		bool getLogFPS() const;

		static const String DEFAULT_INI_FILE_NAME;


	private:

		Root *mRoot;

		IniFile mIniFile;

	};

}


#endif /* INCLUDE_OPTIONS_H_ */
