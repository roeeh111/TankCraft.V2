#pragma once
#include <entt/entt.hpp>
#include <string>
#include <RakPeerInterface.h>
#include "IDTranslationComponent.h"
#include "Tanks.pb.h"
#include "BaseComponent.h"

/*
*  Class containing all of the global data to the system.
*/


namespace GameData {
	class GameData {
	public:
		// The main UI
		// This data is the SAME across all clients
		std::vector<std::vector<char>> map;

		// main registry
		// This data is DIFFERENT across the server and all clients
		entt::registry m_reg;

		// Peer to Peer interface with raknet
		RakNet::RakPeerInterface* rpi;

		// The translation of net to entity structure
		// This data should be SERVER ONLY and ONE PER CLIENT
		std::map<networkID, entt::entity> netToEnttid;

		// The map from each client to its list of entities
		std::map<RakNet::SystemAddress, std::list<networkID>> clientAddressToEntities;

		// The map of entities to components to update
		std::map<networkID, std::list<baseComponent>> updateMap; // DO i need this???

		// the address were connected to
	//	const char* address;
		RakNet::SystemAddress rakAddress;


		// Whether this scene is the server or a client 
		bool isServer;


		// for debugging purposes
		bool first;

		// The message to be displayed at the bottom of the board
		// For debug purposes and notifications
		std::string message;


	};
}