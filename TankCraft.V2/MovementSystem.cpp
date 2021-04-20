#include "Components.h"
#include "MovementSystem.h"
#include "UISystem.h"


/*
	TODO: loop over all entities with a position element
		1) if that entity has a user input element and that user input element is dirty, call move entity
		2) if that entity has a position element (and not a user input) and that position is dirty, call some function

		TODO: write function to move something based on position
		TODO: figure out how were going to do postions dirty 

		NOTE: The server is the one calling updatemovement, so take that into account when designing
*/

// Update movement should only be control based? 

namespace MovementSystem {
	// NOTE: this only moves tanks now, genericize this so that it can move any entity that is dirty
	void MovementSystem::updateMovement(GameData::GameData& data)
	{
		// For now, loop over all entities with position and user input
		// and call moveEntity
		auto view = data.m_reg.view<ComponentView::userInput, ComponentView::position>();
		for (auto &entity : view) {
			auto& uinput = data.m_reg.get<ComponentView::userInput>(entity);

			// We are guarenteed that any user input that needs to be updated will be set dirty=1
			if (uinput.dirty()) {	
				moveEntity(data, entity, data.m_reg.get<ComponentView::userInput>(entity));
			}
		}
	}

	void MovementSystem::moveEntity(GameData::GameData& data, const entt::entity &entity, ComponentView::userInput &input) {
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
	//	input.lock();
		input.setLeft(0); input.setRight(0); input.setUp(0); input.setRight(0);
		input.dirty_ = false;
//		input.unlock(data, entity);		// NOTE: NOT UNLOCKING THE USERINPUT COMPONENT BECAUSE WE DONT WANT TO SEND IT BACK
	}
}
 

// TODO: MAKE SURE TO ONLY CALL UNLOCK ON A COMPONENT IF THAT COMPONENT IS NOW DIRTY (the points and the input above us are the culprits)