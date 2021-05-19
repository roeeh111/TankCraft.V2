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
	class UISystem {
	public:

		void initialize(GameData::GameData& data);

		void updateClient(GameData::GameData& data, HDC hdcBackBuff, PAINTSTRUCT ps, HWND hwnd);

		void updateServer(GameData::GameData& data, HDC hdcBackBuff, PAINTSTRUCT ps, HWND hwnd);

		void serverMessage(GameData::GameData& data, const char* msg);

		void clientMessage(GameData::GameData& data, char* msg);

		// Get input from the user, and set its position component
		//void getUserInput(entt::registry& m_reg, entt::entity& clientEntity); // Might need this later, but has been refactored into other methods

		// Getting keyboard input
		bool getKeyBoardInput(GameData::GameData& data, entt::entity& clientEntity);

		// Update each ui entity's position in the map
		void updateMapPositions(GameData::GameData& data);
	};
}