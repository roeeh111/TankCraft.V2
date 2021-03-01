#pragma once
#include <entt/entt.hpp>
#include "SceneComponent.h"

namespace MovementSystem {
	class MovementSystem {
	public:
		void updateMovement(SceneComponent::SceneComponent& data);

		void moveEntity(entt::registry& m_reg, entt::entity& entity, ComponentView::userInput& input);
	};
}