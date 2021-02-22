#pragma once
#include <entt/entt.hpp>

namespace MovementSystem {
	class MovementSystem {
	public:
		void updateMovement(entt::registry& m_reg, std::vector<std::vector<char>>& map);
	};
}