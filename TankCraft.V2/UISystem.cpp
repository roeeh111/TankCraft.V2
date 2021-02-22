#include "UISystem.h"
#include "Components.h"

namespace UI {
	void UISystem::updateUI(entt::registry& m_reg, std::vector<std::vector<char>> &map)
	{

		// get all elements that have a mapObject and position, and then do the updates
		auto view = m_reg.view<GameView::mapObject, GameView::position>();


		for (auto entity : view) {
			// Get the user input for our object
			getUserInput(m_reg, entity);
		}
	}


	void UISystem::printUI(entt::registry& m_reg, std::vector<std::vector<char>> &map)
	{
		// Clear the current screen
		system("CLS");

		// Print out whatever is in the map
		std::cout << std::endl;
		for (int i = 0; i < HEIGHT; i++) {
			for (int j = 0; j < WIDTH; j++) {
				std::cout << map[i][j] << " ";
			}
			std::cout << std::endl;
		}

		// Print out points and client names of any entities that have those
		auto view = m_reg.view<GameView::score, GameView::clientName>();
		for (auto entity : view) {
			std::cout << "Points for client " << view.get<GameView::clientName>(entity).name << ": " << view.get<GameView::score>(entity).points << std::endl;
		}
	}

	void UISystem::getUserInput(entt::registry& m_reg, entt::entity& clientEntity)
	{

		char input;
		auto& points = m_reg.get<GameView::position>(clientEntity);


		std::cin >> input;

		points.dirty = 1;
		points.prevx = points.curx;
		points.prevy = points.cury;

		if (input == 'a') {
			if (points.curx == 0) {
				points.curx = WIDTH - 1;
			}
			else {
				points.curx--;
			}
		}
		else if (input == 's') {
			if (points.cury == HEIGHT - 1) {
				points.cury = 0;
			}
			else {
				points.cury++;
			}
		}
		else if (input == 'd') {
			if (points.curx == WIDTH - 1) {
				points.curx = 0;
			}
			else {
				points.curx++;
			}
		}
		else if (input == 'w') {
			if (points.cury == 0) {
				points.cury = HEIGHT - 1;
			}
			else {
				points.cury--;
			}
		}
		else {
			points.dirty = 0;
		}
	}




}