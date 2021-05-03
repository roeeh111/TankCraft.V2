#pragma once
#include "GameData.h"
#include "Components.h"

namespace ScoreSystem {
	void updateScore(GameData::GameData& data, entt::entity entity, ComponentView::position& pos);
	void printScore(GameData::GameData& data);
}
