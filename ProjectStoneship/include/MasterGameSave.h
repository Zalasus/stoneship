/*
 * MasterGameSave.h
 *
 *  Created on: Oct 28, 2015
 *      Author: zalasus
 */

#ifndef INCLUDE_MASTERGAMESAVE_H_
#define INCLUDE_MASTERGAMESAVE_H_


namespace Stoneship
{

	class MasterGameSave : public MasterGameFile
	{
	public:

		MasterGameSave(const String &filename, MasterGameFileManager &mgfm);
		~MasterGameSave();

		void load();

	};

}


#endif /* INCLUDE_MASTERGAMESAVE_H_ */
