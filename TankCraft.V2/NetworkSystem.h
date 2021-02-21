#pragma once
#include <entt/entt.hpp>
#include <RakPeerInterface.h>
#include <iostream>
#include "Packet.h"
// TODO: add multithreading library

#define SERVER_PORT 60000

namespace NetworkSystem {
	class NetworkHandler {
	public:

		// TODO: may need a refrence to the m_reg if its not global
		// Go through all incoming packets to the server, and dispatch game changes accordingly
		// Broadcast the results to all clients
		void updateServer(entt::registry &m_reg, RakNet::RakPeerInterface* peer);

		// Go through all incomming game updates, and update the client's game accordingly
		// In addition, go through all controls inputted, and send them up to the server
		void updateClient(entt::registry &m_reg, RakNet::RakPeerInterface* peer);

		// Connect to the given server address as a client
		bool clientConnect(RakNet::RakPeerInterface* peer, unsigned short port, const char* hostAddress);

	private:

		// Deserealizes the packet, checks that it is a game update packet.
		// If packet is a game update, aquires the lock to the component, and updates the data of that component
		void makeClientUpdate(entt::registry &m_reg, RakNet::Packet* pack);

		// Depending on the type of packet and its valididy, will execute the control
		// commands given by the packet.
		// Execution of those controls will append new components to the changeQueue
		void makeServerUpdate(entt::registry &m_reg, RakNet::Packet* pack);

		// add an entity to the server or client
		void addEntity(entt::registry& m_reg, bool isServer);

		// remove an entity from the server or client
		void removeEntity(bool isServer);

		// Server handling a disconnecting client
		// Delete entity from the registry and clean up metadata
		void handleDisconnect(bool isServer);

		// Server handling an incomming connecting client
		// add a new connection
		void handleConnection();
	};
}