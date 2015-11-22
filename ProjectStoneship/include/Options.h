/*
 * Options.h
 *
 *  Created on: Nov 9, 2015
 *      Author: zalasus
 */

#ifndef INCLUDE_OPTIONS_H_
#define INCLUDE_OPTIONS_H_

#include "String.h"

namespace Stoneship
{

	class Root;

	class Options
	{
	public:

		Options(Root *root);

		void load(const String &path);


	private:

		Root *mRoot;

	};

}


#endif /* INCLUDE_OPTIONS_H_ */
