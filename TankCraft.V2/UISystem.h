#pragma once
#include <iostream>
#include <entt/entt.hpp>
#define WIDTH 20
#define HEIGHT 20


namespace UI {
	class UISystem {
	public:
		void updateUI(entt::registry& m_reg, std::vector<std::vector<char>> &map);

		// print out the current game map to console
		void printUI(entt::registry& m_reg, std::vector<std::vector<char>> &map);

		// Get input from the user, and set its position component
		// Also, set the dirty bit = 1								TODO:: can improve this by not having to loop over all components with position, 
		void getUserInput(entt::registry& m_reg, entt::entity& clientEntity);				//		But all components that have position and are dirty (or some signaling)

	};
}