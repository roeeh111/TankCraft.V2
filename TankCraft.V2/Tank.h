#pragma once
#include "GameData.h"

namespace Tank {
	void addTank(GameData::GameData& data, RakNet::Packet* pack, std::string& loginName);
}