#pragma once
#include <entt/entt.hpp>
#include "SceneData.h"

namespace MovementSystem {
	class MovementSystem {
	public:
		void updateMovement(SceneData::SceneData& data);
	};
}