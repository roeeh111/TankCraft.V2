#pragma once
#include "GameData.h"

//Police entity
/*
Get mobbed coins back, has a position, has a map object
*/

namespace Police {
	//	// Spawn a police with zero coins at a position on the map
	void addPolice(GameData::GameData& data, RakNet::Packet* pack, uint32_t points, uint32_t x, uint32_t y);
}