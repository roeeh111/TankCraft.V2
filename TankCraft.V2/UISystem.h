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

		// Getting keyboard input
		bool getKeyBoardInput(GameData::GameData& data, entt::entity& clientEntity);

	};
}