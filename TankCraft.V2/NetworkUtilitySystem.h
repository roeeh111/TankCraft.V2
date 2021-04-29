#pragma once
#include <entt/entt.hpp>
#include <RakPeerInterface.h>
#include <iostream>
#include "GameData.h"
#include "Components.h"
#include "IDTranslationSystem.h"
#include "IDTranslationComponent.h"
#include "MovementSystem.h"

// TODO: add multithreading to the server

#define RELIABILITY RELIABLE_ORDERED
#define SERVER_PORT 60000

namespace NetworkUtilitySystem {
	// add an entity to the server or client
	entt::entity addEntity(GameData::GameData& data, RakNet::Packet* pack, bool isServer, bool responding);

	void removeEntity(GameData::GameData& data, RakNet::Packet* pack, networkID remID, bool isServer, bool responding);

	void sendClientInput(GameData::GameData& data, RakNet::Packet* pack);

	// Handle an incoming control packet
	void handleControl(GameData::GameData& data, RakNet::Packet* pack);

	// send a control packet to the server
	void sendControl(GameData::GameData& data, ComponentView::userInput& usrInput, networkID netid);

	// Broadcast the contents of a bistream to all connections in the connections list
	void broadcast(GameData::GameData& data, const RakNet::BitStream* bitStream, PacketPriority priority, PacketReliability reliability, char orderingChannel);

	// Handle an incomming game update packet
	void handleGameUpdate(GameData::GameData& data, RakNet::Packet* pack);
}