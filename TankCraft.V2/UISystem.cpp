#include "UISystem.h"
#include "Components.h"

namespace UI {
	void UISystem::updateUI(SceneData::SceneData& data)
	{

		// get all elements that have a mapObject and position, and then do the updates
		auto view = data.m_reg.view<ComponentView::mapObject, ComponentView::position>();


		for (auto entity : view) {
			// Get the user input for our object
			getUserInput(data.m_reg, entity);
		}
	}


	void UISystem::printUI(SceneData::SceneData& data)
	{
		// Clear the current screen
		system("CLS");

		// Print out whatever is in the map
		std::cout << std::endl;
		for (int i = 0; i < HEIGHT; i++) {
			for (int j = 0; j < WIDTH; j++) {
				std::cout << data.map[i][j] << " ";
			}
			std::cout << std::endl;
		}

		// Print out points and client names of any entities that have those
		auto view = data.m_reg.view<ComponentView::score, ComponentView::clientName>();
		for (auto entity : view) {
			std::cout << "Points for client " << view.get<ComponentView::clientName>(entity).name << ": " << view.get<ComponentView::score>(entity).points << std::endl;
		}
	}

	void UISystem::getUserInput(entt::registry& m_reg, entt::entity& clientEntity)
	{

		char input;
		auto& points = m_reg.get<ComponentView::position>(clientEntity);


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