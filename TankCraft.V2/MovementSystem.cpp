#include "Components.h"
#include "MovementSystem.h"
#include "UISystem.h"
#include "ScoreSystem.h"
#include "HealthAndDamageSystem.h"
//#include "Tran"



namespace MovementSystem {

	void MovementSystem::init(GameData::GameData& data) {
		// No init function
	}

	// NOTE: this only moves tanks now, genericize this so that it can move any entity that is dirty
	void MovementSystem::update(GameData::GameData& data)
	{
		// Only handles movement in server
		if (!data.isServer) { return; }
	}

	void MovementSystem::moveEntity(GameData::GameData& data, const entt::entity& entity, ComponentView::userInput& input) {
		std::cout << "Moving entitiy: " << (int) entity << std::endl;
		std::cout << "left = " << input.left() << " right = " << input.right() << " up = " << input.up() << " down = " << input.down() << std::endl;
		std::cout << std::endl;
		auto& points = data.m_reg.get<ComponentView::position>(entity);
		points.lock(); // lock the component

		points.setPrevx(points.curx());
		points.setPrevy(points.cury());
		if (input.left()) {
			if (points.curx() <= 0) {
				points.setCurx(WIDTH - 1);
			}
			else if (data.map[points.cury()][points.curx() - 1] != 1) {
				points.setCurx(points.curx() - 1);
			}
		}
		else if (input.down()) {
			if (points.cury() >= HEIGHT - 1) {
				points.setCury(0);
			}
			else if (data.map[points.cury() + 1][points.curx()] != 1) {
				points.setCury(points.cury() + 1);

			}
		}
		else if (input.right()) {
			if (points.curx() >= WIDTH - 1) {
				points.setCurx(0);
			}
			else if (data.map[points.cury()][points.curx() + 1] != 1) {
				points.setCurx(points.curx() + 1);
			}
		}
		else if (input.up()) {
			if (points.cury() <= 0) {
				points.setCury(HEIGHT - 1);
			}
			else if (data.map[points.cury() - 1][points.curx()] != 1) {
				points.setCury(points.cury() - 1);

			}
		}

		points.unlock(data, entity);


		// Clear the input 
		input.setLeft(0); input.setRight(0); input.setUp(0); input.setRight(0);
		input.dirty_ = false;
		// NOTE: NOT UNLOCKING THE USERINPUT COMPONENT BECAUSE WE DONT WANT TO SEND IT BACK
	}



	void moveMobs(GameData::GameData& data)
	{
		auto view = data.m_reg.view<ComponentView::mapObject, ComponentView::position, ComponentView::velocity>();
		for (auto entity : view) {
		//	std::cout << "moving mob " << 
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
						if (data.map[pos.cury()][pos.curx() + 1] != 1) {
							pos.setCurx(pos.curx() + 1);		//
						}
						else {
							vel.setDx(-1*vel.getDx());
						//	pos.setCurx(pos.curx() - 1);		//
						}

					}
				}
				else {
					if (pos.curx() <= 0) {
						pos.setCurx(WIDTH - 1);
					}
					else {
						if (data.map[pos.cury()][pos.curx() - 1] != 1) {
							pos.setCurx(pos.curx() - 1);				//
						}
						else {
							vel.setDx(-1 * vel.getDx());
						//	pos.setCurx(pos.curx() + 1);				//
						}

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
						if (data.map[pos.cury() + 1][pos.curx()] != 1) {
							pos.setCury(pos.cury() + 1);			//
						}
						else {
						//	pos.setCury(pos.cury() - 1);			//
							vel.setDy(-1 * vel.getDy());
						}
					}
				}
				else {
					if (pos.cury() <= 0) {
						pos.setCury(HEIGHT - 1);
					}
					else {
						if (data.map[pos.cury() - 1][pos.curx()] != 1) {
							pos.setCury(pos.cury() - 1);		//
						}
						else {
						//	pos.setCury(pos.cury() + 1);		//
							vel.setDy(-1 * vel.getDy());
						}

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
