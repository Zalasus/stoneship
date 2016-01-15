/*
 * StoneshipConstants.h
 *
 *  Created on: 28.08.2015
 *      Author: Zalasus
 */

#ifndef INCLUDE_STONESHIPDEFINES_H_
#define INCLUDE_STONESHIPDEFINES_H_

// ---------- Game/Engine specific defines ----------

#define STONESHIP_VERSION "3.0.0"
#define STONESHIP_GAMEID "Stoneship"
#define STONESHIP_DEFAULT_WINDOW_CAPTION "Stoneship"


//---------- Performance oriented stuff -------------

//reduced the amount of file stream seekg() used in the code for filesystems with heavy caching or
//other factors which make seekg less perfomant for forward searches than skipping byte by byte
#define STONESHIP_IO_AVOID_SEEKS


//---------- System specific defines ----------

#ifdef WIN32
    #define STONESHIP_SYSTEM_DIR_SEP '\\'
#else
    #define STONESHIP_SYSTEM_DIR_SEP '/'
#endif



#endif /* INCLUDE_STONESHIPDEFINES_H_ */
