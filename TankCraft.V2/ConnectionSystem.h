#pragma once
#include <entt/entt.hpp>
#include <RakPeerInterface.h>
#include <iostream>
#include "GameData.h"
#include "Components.h"
#include "PrimarySystem.h"
#include "IDTranslationSystem.h"
#include "IDTranslationComponent.h"
#include "MovementSystem.h"

// TODO: add multithreading to the server

#define RELIABILITY RELIABLE_ORDERED
#define SERVER_PORT 60000

namespace ConnectionSystem {
	class ConnectionSystem {
	public:
		// TODO: may need a refrence to the m_reg if its not global
		// Go through all incoming packets to the server, and dispatch game changes accordingly
		// Broadcast the results to all clients
		void updateServer(GameData::GameData& data);

		// Go through all incomming game updates, and update the client's game accordingly
		// In addition, go through all controls inputted, and send them up to the server
		void updateClient(GameData::GameData& data);

		// Connect to the given server address as a client
		bool clientConnect(RakNet::RakPeerInterface* peer, unsigned short port, const char* hostAddress);

		// Disconnect from the given server address as a client
		void clientDisconnect(RakNet::RakPeerInterface* peer, const char* hostAddress);

		// Server handling an incomming connecting client
		// add a new connection
		void handleConnection(GameData::GameData& data, RakNet::Packet* pack);

		// Server handling a disconnecting client
		// NOT USED right now
		void handleDisconnect(GameData::GameData& data, RakNet::Packet* pack);

		// Server handling a lost client
		// TODO: Figure out the behavior of lost connection
		void handleLostConnection(GameData::GameData& data, RakNet::Packet* pack);

		// Send a login packet to the server with the given username
		void sendLoginPacket(GameData::GameData& data, std::string& name);

		// Handle getting a login packet
		void handleLogin(GameData::GameData& data, RakNet::Packet* pack);

		// Handle an incoming game update packet
		void handleGameUpdate(GameData::GameData& data, RakNet::Packet* pack);

	};

}