#pragma once
#include "GameData.h"
#include "Components.h"

namespace HealthAndDamageSystem {
	void printHealth(GameData::GameData& data);
	void updateDamage(GameData::GameData& data, entt::entity entity, ComponentView::position& pos);
}