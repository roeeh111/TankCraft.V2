#pragma once
#include <entt/entt.hpp>
#include <RakPeerInterface.h>
#include "NetworkFields.h"

namespace SceneData {
	class SceneData {
	public:
		// The main UI
		std::vector<std::vector<char>> map;

		// main registry
		entt::registry m_reg;

		// The interface with raknet 
		RakNet::RakPeerInterface* rpi;


		// The translation of net to entity structure
		std::map<networkID, entt::entity> netToEnttid;

		// Whether this scene is the server or a client 
		bool isServer;

	};
}