#include "HealthAndDamageSystem.h"

namespace HealthAndDamageSystem {
	void printHealth(GameData::GameData& data)
	{
		auto view = data.m_reg.view<ComponentView::health, ComponentView::clientName>();
		for (auto entity : view) {
			std::cout << "Health for client " << view.get<ComponentView::clientName>(entity).name() << ": " << view.get<ComponentView::health>(entity).hp() << std::endl;
		}
	}

	void updateDamage(GameData::GameData& data, entt::entity entity, ComponentView::position& pos)
	{
		if (data.map[pos.cury()][pos.curx()] == 'S') {
			if (data.m_reg.has<ComponentView::health>(entity)) {
				// Get the entities health
				auto& health = data.m_reg.get<ComponentView::health>(entity);
				health.lock();

				// Get the damage entity that is at our location
				auto view = data.m_reg.view<ComponentView::damageDone, ComponentView::mapObject, ComponentView::position>();
				for (auto damageEntity : view) {
					auto& damagePos = view.get<ComponentView::position>(damageEntity);

					// if were in the same place
					if (pos.curx() == damagePos.curx() && pos.cury() == damagePos.cury()) {
						// Decrement the players health by the damage caused by the spikes
						health.setHp(health.hp() - view.get<ComponentView::damageDone>(damageEntity).damage);
					}
				}
				health.unlock(data, entity);
			}
		}
	}



}

