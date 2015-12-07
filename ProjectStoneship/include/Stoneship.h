/*
 * Stoneship.h
 *
 *  Created on: Nov 24, 2015
 *      Author: zalasus
 */

#ifndef INCLUDE_STONESHIP_H_
#define INCLUDE_STONESHIP_H_

#ifndef _DEBUG
#warning Use of the Stoneship.h header for other targets than Debug is discouraged
#endif


#include <IActor.h>
#include <IEntity.h>
#include <IEntityBase.h>
#include "EntityManager.h"
#include "EventPipeline.h"
#include "Exception.h"
#include "Inventory.h"
#include <IEntityBaseItem.h>
#include "Logger.h"
#include "MasterGameFile.h"
#include "MGFDataReader.h"
#include "MGFDataWriter.h"
#include "MGFManager.h"
#include "Options.h"
#include "Player.h"
#include "Record.h"
#include "ResourceManager.h"
#include "Root.h"
#include "SaveGameFile.h"
#include <StoneshipDefines.h>
#include "String.h"
#include "Types.h"
#include <IWorld.h>
#include <IEntityBaseWorld.h>
#include "WorldManager.h"
#include "WorldDungeon.h"
#include "WorldOutdoor.h"

#endif /* INCLUDE_STONESHIP_H_ */
