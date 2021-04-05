#include "Components.h"
#include "MovementSystem.h"
#include "UISystem.h"


namespace MovementSystem {
	void updateMovement(GameData::GameData& data)
	{
		// For now, loop over all entities with position and user input
		// and call moveEntity
		auto view = data.m_reg.view<ComponentView::userInput, ComponentView::position>();
		for (auto &entity : view) {
			moveEntity(data, entity, data.m_reg.get<ComponentView::userInput>(entity));
		}

		// TODO:
		// Networked version: if user input is dirty, signal networking system to send control packet
		// The acutal update will happen from the network calling moveEntity
	}

	void moveEntity(GameData::GameData& data, const entt::entity &entity, ComponentView::userInput &input) {
		auto& points = data.m_reg.get<ComponentView::position>(entity);
		points.lock(); // lock the component


		points.setPrevx(points.curx());
		points.setPrevy(points.cury());

		if (input.left()) {
			if (points.curx() <= 0) {
				points.setCurx(WIDTH - 1);
			}
			else {
				points.setCurx(points.curx() - 1);
			}
		}
		else if (input.down()) {
			if (points.cury() >= HEIGHT - 1) {
				points.setCury(0);
			}
			else {
				points.setCury(points.cury() + 1);

			}
		}
		else if (input.right()) {
			if (points.curx() >= WIDTH - 1) {
				points.setCurx(0);
			}
			else {
				points.setCurx(points.curx() + 1);
			}
		}
		else if (input.up()) {
			if (points.cury() <= 0) {
				points.setCury(HEIGHT - 1);

			}
			else {
				points.setCury(points.cury() - 1);

			}
		}
		points.unlock(data, entity);

		// Clear the input 
		input.lock();
		input.networked = 0; input.setLeft(0); input.setRight(0); input.setUp(0); input.setRight(0);
		input.unlock(data, entity);
	}
}
