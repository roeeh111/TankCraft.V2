#pragma once
#include <entt/entt.hpp>
#include <chrono>
#include <RakPeerInterface.h>
#include "Components.h"
#include "GameData.h"
#include "UISystem.h"
#include "IDTranslationSystem.h"
#include "NetworkSystem.h"
#include "MovementSystem.h"


/*
Lagging and waiting issue in movement:
 -> first figure out whats buging with the controls
 -> multithread! let the movement system be its own thread, and the ui system be its own thread. that way, theres no blocking between them
	- will need to lock the controls component and position component?
*/
namespace GameAdmin {

	// The main scene handles the state change of the main game.
	// We can possibly have "live scene", "replay scene", etc.
	class MainScene {
	public:
		// Scan through each client, check if its dirty bit is set and change data if it is
		void update();

		// Constructor, starts up rackNet with inputted max clients
		MainScene(bool isServer_, uint32_t maxClients);

		~MainScene();

		// Data is stored in the component
		GameData::GameData data;

		// time stamp for testing purposes
		std::chrono::seconds test; 

	private:
		void initNetworkSystem(bool isServer_, uint32_t maxClients);
		void initIDTranslationSystem(bool isServer);
		void initUISystem();
		void initMovementSystem();
		void initMessageSystem();

		void clientLogin();
		void serverLogin(uint32_t maxClients);
	};
}