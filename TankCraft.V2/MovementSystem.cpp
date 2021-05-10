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

	void MovementSystem::init(GameData::GameData& data) {
		// No init function
	}

	// NOTE: this only moves tanks now, genericize this so that it can move any entity that is dirty
	void MovementSystem::update(GameData::GameData& data)
	{
		// Only handles movement in server
		if (!data.isServer) { return; }
		// For now, loop over all entities with position and user input
		// and call moveEntity
/*		auto view = data.m_reg.view<ComponentView::userInput, ComponentView::position>();
		for (auto &entity : view) {
			auto& uinput = data.m_reg.get<ComponentView::userInput>(entity);
			std::cout << "updating movement for entity " << (int)entity << std::endl;

			// We are guarenteed that any user input that needs to be updated will be set dirty=1
			if (uinput.dirty()) {	
				moveEntity(data, entity, data.m_reg.get<ComponentView::userInput>(entity));
			}
		}
		*/
	}

	void MovementSystem::moveEntity(GameData::GameData& data, const entt::entity& entity, ComponentView::userInput& input) {
		//std::cout << "Moving entitiy: " << (int) entity << std::endl;
		//std::cout << "left = " << input.left() << " right = " << input.right() << " up = " << input.up() << " down = " << input.down() << std::endl;
		std::cout << std::endl;
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

		//std::cout << "After move: " << "prevx = " << points.prevx() << " prevy = " << points.prevy() << " curx = " << points.curx() << " cury = " << points.cury() << std::endl;

		// Clear the input 
	//	input.lock();
		input.setLeft(0); input.setRight(0); input.setUp(0); input.setRight(0);
		input.dirty_ = false;
//		input.unlock(data, entity);		// NOTE: NOT UNLOCKING THE USERINPUT COMPONENT BECAUSE WE DONT WANT TO SEND IT BACK
	}



	void moveMobs(GameData::GameData& data)
	{
		auto view = data.m_reg.view<ComponentView::mapObject, ComponentView::position, ComponentView::velocity>();
		for (auto entity : view) {
			// Get the entities velocity
			auto& vel = view.get<ComponentView::velocity>(entity);
			auto& pos = view.get<ComponentView::position>(entity);

			pos.setPrevx(pos.curx());
			pos.setPrevy(pos.cury());

			// if the velocities count = dx or dy, move the entity
			if (abs(vel.getDx()) == vel.getxCount()) {
				if (vel.getDx() > 0) {
					if (pos.curx() >= WIDTH-1) {
						pos.setCurx(0);
					}
					else {
						pos.setCurx(pos.curx() + 1);
					}
				}
				else {
					if (pos.curx() <= 0) {
						pos.setCurx(WIDTH - 1);
					}
					else {
						pos.setCurx(pos.curx() - 1);
					}
				}
				vel.setxCount(0);
			}
			else {
				vel.setxCount(vel.getxCount() + 1);
			}

			if (vel.getDy() == vel.getyCount()) {
				if (vel.getDy() > 0) {
					if (pos.cury() >= HEIGHT - 1) {
						pos.setCury(0);
					}
					else {
						pos.setCury(pos.cury() + 1);
					}
				}
				else {
					if (pos.cury() <= 0) {
						pos.setCury(HEIGHT - 1);
					}
					else {
						pos.setCury(pos.cury() - 1);
					}
				}
				vel.setyCount(0);
			}
			else {
				vel.setyCount(vel.getyCount() + 1);
			}

			vel.unlock(data, entity);
			pos.unlock(data, entity);

		}
	}
}
