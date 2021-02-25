#pragma once
#include <entt/entt.hpp>
#include "SceneData.h"

namespace MovementSystem {
	class MovementSystem {
	public:
		void updateMovement(SceneData::SceneData& data);

		void moveEntity(entt::registry& m_reg, entt::entity& entity, ComponentView::userInput& input);
	};
}