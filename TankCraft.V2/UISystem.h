#pragma once
#include <iostream>
#include <entt/entt.hpp>
#include "SceneComponent.h"
#define WIDTH 20
#define HEIGHT 20

/*
*  The system for getting a user's input, and printing the games state to the console.
* 
*/

namespace UI {
	class UISystem {
	public:
		void updateUI(SceneComponent::SceneComponent& data);

		// print out the current game map to console
		void printUI(SceneComponent::SceneComponent& data);

		// Get input from the user, and set its position component
//		void getUserInput(entt::registry& m_reg, entt::entity& clientEntity); // Might need this later, but has been refactored into other methods

		// Add a tank entity to the game
		void addTank(SceneComponent::SceneComponent& data, std::string clientName_);

		// Getting keyboard input
		void getKeyBoardInput(entt::registry& m_reg, entt::entity& clientEntity);


		// Update each ui entity's position in the map
		void updateMapPositions(SceneComponent::SceneComponent& data);

	};
}