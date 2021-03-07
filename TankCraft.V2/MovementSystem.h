#pragma once
#include <entt/entt.hpp>
#include "GameData.h"

namespace MovementSystem {
	class MovementSystem {
	public:
		void updateMovement(GameData::GameData& data);

		void moveEntity(entt::registry& m_reg, entt::entity& entity, ComponentView::userInput& input);
	};
}

/*
*  System for moving an entity with a position around.
* 
*/