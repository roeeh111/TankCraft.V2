#pragma once
#include "GameData.h"

namespace Coins {
	void addCoins(GameData::GameData& data, RakNet::Packet* pack, uint32_t points, uint32_t x, uint32_t y);
}