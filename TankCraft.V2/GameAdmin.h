#pragma once
#include <entt/entt.hpp>
#include <chrono>
#include <RakPeerInterface.h>
#include "Components.h"
#include "SceneComponent.h"
#include "UISystem.h"
#include "IDTranslationSystem.h"
#include "NetworkSystem.h"
#include "MovementSystem.h"

/*
*  The main "system" and data in our scene.
* 
*/


/*
 To fix our lagging and waiting issue:
 -> first figure out whats buging with the controls
 -> multithread! let the movement system be its own thread, and the ui system be its own thread. that way, theres no blocking between them
	- will need to lock the controls component and position component?
*/

namespace GameAdmin {

	// Tank scene is the scene for the tank game.
	// All scenes are managed within the scene system
	class TanksScene {
	public:
		// Scan through each client, check if its dirty bit is set and change data if it is
		void update();

		// Constructor, starts up rackNet with inputted max clients
		TanksScene(bool isServer_, uint32_t maxClients);

		~TanksScene();

		// Data is stored in the component
		SceneComponent::SceneComponent data;

		// Systems
		UI::UISystem uiSystem;
		TranslationSystem::IDTranslation translationSystem;
		NetworkSystem::NetworkHandler netSystem;
		MovementSystem::MovementSystem movSystem;

		// time stamp for testing purposes
		std::chrono::seconds test; 

	private:
		void initNetworkSystem(bool isServer_, uint32_t maxClients);
		void initIDTranslationSystem();
		void initUISystem();
		void initMovementSystem();
	};
}