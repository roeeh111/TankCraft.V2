#pragma once
#include <entt/entt.hpp>
#include <RakPeerInterface.h>
#include "IDTranslationComponent.h"
#include <string>

namespace SceneComponent {
	class SceneComponent {
	public:
		// The main UI
		std::vector<std::vector<char>> map;

		// main registry
		entt::registry m_reg;

		// The interface with raknet 
		RakNet::RakPeerInterface* rpi;

		// The translation of net to entity structure
		std::map<networkID, entt::entity> netToEnttid;

		// The map from each client to its list of entities
		std::map<RakNet::SystemAddress, std::list<entt::entity>> clientAddressToEntities;

		// The message to be displayed at the bottom of the board
		// Fro debug purposes and notifications
		std::string message;

		// Whether this scene is the server or a client 
		bool isServer;

		// TODO: keep track of the server's address
	};
}