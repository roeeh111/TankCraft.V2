#pragma once
//#include <list>
#include <entt/entt.hpp>
#include <RakPeerInterface.h>
#include "Components.h"
#include "SceneComponent.h"

/*
 To fix our lagging and waiting issue:
 -> first figure out whats buging with the controls
 -> multithread! let the movement system be its own thread, and the ui system be its own thread. that way, theres no blocking between them
	- will need to lock the controls component and position component?
*/




namespace GameView {

	// Try and break this down into two classes, one for tankssystems, one for tanksData

	// If a system wants to do something, pass it an instance of tanksData
	// This class contains both those classes

	// if scene wants to update, it calls scene.update (gets data from scene data, and the system functions from scene systems)
	
	// if a system wants to do something by its own, pass in scenedata

	// if data wants to be data, let it be data
	class TanksScene {
	public:
		// Scan through each client, check if its dirty bit is set and change data if it is
		void update();

		// Constructor, starts up rackNet with inputted max clients
		TanksScene(bool isServer_, uint32_t maxClients);

		~TanksScene();

		// Data
		SceneComponent::SceneComponent data;

		// Systems
		// SceneSystems::SceneSystems systems;


	private:

		void initNetworkSystem(bool isServer_, uint32_t maxClients);
		void initIDTranslationSystem();
		void initUISystem();
		void initMovementSystem();


	};
}


