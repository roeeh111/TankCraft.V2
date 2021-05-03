#pragma once
#include "GameData.h"
#include "Components.h"

namespace Spikes {
	

	/*
		- Map object component
		- damage component
		- position component
	
	*/
	void addSpikes(GameData::GameData& data, RakNet::Packet* pack, uint32_t damage, uint32_t x, uint32_t y);

	// Fill in a function that will cause damage to a character
//	void updateSpikes(GameData::GameData& data, entt::entity entity, ComponentView::position& pos); // How are we causeing damage to another entity???
}