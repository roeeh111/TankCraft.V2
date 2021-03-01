#include "Components.h"
#include "MovementSystem.h"
#include "UISystem.h"

#include "UISystem.h"
namespace MovementSystem {
	void MovementSystem::updateMovement(SceneComponent::SceneComponent& data)
	{
		// For now, loop over all entities with position and user input
		// and call moveEntity
		auto view = data.m_reg.view<ComponentView::userInput, ComponentView::position>();
		for (auto entity : view) {
			moveEntity(data.m_reg, entity, data.m_reg.get<ComponentView::userInput>(entity));
		}

		

		// TODO:
		// Networking version: if user input is dirty, signal networking system to send control packet
		// The acutal update will happen from the network calling moveEntity
	}

	void MovementSystem::moveEntity(entt::registry &m_reg, entt::entity &entity, ComponentView::userInput &input) {
		auto& points = m_reg.get<ComponentView::position>(entity);


		points.prevx = points.curx;
		points.prevy = points.cury;

		if (input.left) {
			if (points.curx == 0) {
				points.curx = WIDTH - 1;
			}
			else {
				points.curx--;
			}
		}
		else if (input.down) {
			if (points.cury == HEIGHT - 1) {
				points.cury = 0;
			}
			else {
				points.cury++;
			}
		}
		else if (input.right) {
			if (points.curx == WIDTH - 1) {
				points.curx = 0;
			}
			else {
				points.curx++;
			}
		}
		else if (input.up) {
			if (points.cury == 0) {
				points.cury = HEIGHT - 1;
			}
			else {
				points.cury--;
			}
		}

		// Clear the input 
		input.dirty = 0; input.left = 0; input.right = 0; input.up = 0; input.right = 0;
	}
}
