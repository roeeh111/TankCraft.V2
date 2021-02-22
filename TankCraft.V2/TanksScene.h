#pragma once
//#include <list>
#include <entt/entt.hpp>
#include <RakPeerInterface.h>
#include "Components.h"
#include "NetworkSystem.h"
#include "IDTranslationSystem.h"
#include "UISystem.h"
#include "MovementSystem.h"

namespace GameView {

	class TanksScene {
	public:
		// Scan through each client, check if its dirty bit is set and change data if it is
		void update();

		void addClient(std::string clientName_);

		// Constructor, starts up rackNet with inputted max clients
		TanksScene(bool isServer_, uint32_t maxClients);

		~TanksScene();

	private:

		void initNetworkSystem(bool isServer_, uint32_t maxClients);
		void initIDTranslationSystem();
		void initUISystem();
		void initMovementSystem();

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

		// SYSTEMS:				TODO (may want to set them as friend functions. So that data transfer will be smoother)
		NetworkSystem::NetworkHandler netSystem;
		TranslationSystem::IDTranslation translationSystem;
		UI::UISystem uiSystem;
		MovementSystem::MovementSystem movSystem;
	};
}


