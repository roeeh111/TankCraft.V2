#pragma once
#include <iostream>
#include <entt/entt.hpp>
#include "GameData.h"
#define WIDTH 20
#define HEIGHT 20

/*
*  The system for getting a user's input, and printing the games state to the console.
* 
*/

namespace UISystem {
	class UISystem : public PrimarySystem::PrimarySystem {
	public:

		void init(GameData::GameData& data);

		void update(GameData::GameData& data);

		// print out the current game map to console
		void printUI(GameData::GameData& data);

		// Get input from the user, and set its position component
		//void getUserInput(entt::registry& m_reg, entt::entity& clientEntity); // Might need this later, but has been refactored into other methods

		// Getting keyboard input
		bool getKeyBoardInput(GameData::GameData& data, entt::entity& clientEntity);

		// Update each ui entity's position in the map
		void updateMapPositions(GameData::GameData& data);
	};
}