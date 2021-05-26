#pragma once
#include "GameData.h"

// Zombie entity:
/*
Does damage, has a position, has velocity, has a map object, has health
*/

namespace Zombie {
	// Spawn a zombie with a certain damage, velocity, and health in a random position on the map
	void addZombie(GameData::GameData& data, RakNet::Packet* pack, uint32_t damageDone, uint32_t health, uint32_t dx, uint32_t dy, uint32_t x, uint32_t y);
}