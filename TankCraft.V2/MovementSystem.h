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

	// Move all mobs with velocity depending on their count field
	void moveMobs(GameData::GameData& data);
}

/*
*  System for moving an entity with a position around.
* 
*/