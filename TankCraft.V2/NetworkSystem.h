#pragma once
#include <entt/entt.hpp>
#include <RakPeerInterface.h>
#include "Packet.h"
#include <iostream>
#include "SceneComponent.h"
#include "IDTranslationSystem.h"
#include "IDTranslationComponent.h"

//#include "Packet.h"
// TODO: add multithreading library

// Problem = we have a cyclic header decleration

#define RELIABILITY RELIABLE_ORDERED
#define SERVER_PORT 60000


namespace NetworkSystem {
	class NetworkHandler {
	public:

		// TODO: may need a refrence to the m_reg if its not global
		// Go through all incoming packets to the server, and dispatch game changes accordingly
		// Broadcast the results to all clients
		void updateServer(SceneComponent::SceneComponent& data, TranslationSystem::IDTranslation& transSystem);

		// Go through all incomming game updates, and update the client's game accordingly
		// In addition, go through all controls inputted, and send them up to the server
		void updateClient(SceneComponent::SceneComponent& data, TranslationSystem::IDTranslation& transSystem);

		// Connect to the given server address as a client
		bool clientConnect(RakNet::RakPeerInterface* peer, unsigned short port, const char* hostAddress);

		// Disconnect from the given server address as a client
		void clientDisconnect(RakNet::RakPeerInterface* peer, const char* hostAddress);



		// Deserealizes the packet, checks that it is a game update packet.
		// If packet is a game update, aquires the lock to the component, and updates the data of that component
		void makeClientUpdate(entt::registry &m_reg, RakNet::Packet* pack);

		// Depending on the type of packet and its valididy, will execute the control
		// commands given by the packet.
		// Execution of those controls will append new components to the changeQueue
		void makeServerUpdate(entt::registry &m_reg, RakNet::Packet* pack);

		// add an entity to the server or client
		void addEntity(SceneComponent::SceneComponent& data, TranslationSystem::IDTranslation& transSystem, RakNet::Packet* pack, bool isServer);

		void removeEntity(SceneComponent::SceneComponent& data, TranslationSystem::IDTranslation& system, networkID netId, bool isServer);

		// Server handling a disconnecting client
		// Delete entity from the registry and clean up metadata
		void handleDisconnect(std::map<RakNet::SystemAddress, std::list<networkID>>& clientAddressToEntities, RakNet::SystemAddress& systemAddress, entt::registry& m_reg);

		// Server handling an incomming connecting client
		// add a new connection
		void handleConnection(std::map<RakNet::SystemAddress, std::list<networkID>>& clientAddressToEntities, RakNet::SystemAddress& systemAddress);

		// Server handling a lost client
		// TODO: Figure out the behavior of this function
		void handleLost(std::map<RakNet::SystemAddress, std::list<networkID>>& clientAddressToEntities, RakNet::SystemAddress& systemAddress);
	};
}