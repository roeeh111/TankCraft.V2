#pragma once
#include <entt/entt.hpp>
#include "GameData.h"

namespace MovementSystem {
	class MovementSystem : public PrimarySystem::PrimarySystem{
	public:
		void init(GameData::GameData& data);

		void update(GameData::GameData& data);

		void moveEntity(GameData::GameData& data, const entt::entity& entity, ComponentView::userInput& input);
	};
}

/*
*  System for moving an entity with a position around.
* 
*/