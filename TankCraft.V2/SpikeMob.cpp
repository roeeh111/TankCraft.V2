#include "SpikeMob.h"
#include "NetworkUtilitySystem.h"

namespace Spikes {



	// HMM, how do we want to signal to the system that a new entity is added??
	// (i say, when the game starts, just spawn in like 4 as part of the login proces)
	void addSpikes(GameData::GameData& data, RakNet::Packet* pack, uint32_t damage, uint32_t x, uint32_t y)
	{
		auto clientEntity = NetworkUtilitySystem::addEntity(data, pack, true, true);
		std::cout << "Adding spikes for entity" << TranslationSystem::getNetId(data, clientEntity) << std::endl;

		// Add the components to the the registry
		(data.m_reg.emplace<ComponentView::mapObject>(clientEntity, 'S' ,true)).unlock(data, clientEntity);
		(data.m_reg.emplace<ComponentView::position>(clientEntity, true, x, y)).unlock(data, clientEntity);
		(data.m_reg.emplace<ComponentView::damageDone>(clientEntity, damage)).unlock(data, clientEntity);

		// DEBUG: print out the map
	//	for (auto* comp : data.updateMap[TranslationSystem::getNetId(data, clientEntity)]) {			// OK problem, damage isnt in the map inside this funciton
	//		comp->print();
	//	}
	//	std::cout << std::endl;
	}

	/* DEPRECATED
	void updateSpikes(GameData::GameData& data, entt::entity entity, ComponentView::position& pos)
	{
		if (data.map[pos.cury()][pos.curx()] == 'S') {
			//std::cout << "Updating spikes, some entity has stepped on a spike" << std::endl;

			if (data.m_reg.has<ComponentView::health>(entity)) {
				// Get the entities health
				auto& health = data.m_reg.get<ComponentView::health>(entity);
				health.lock();

				// Get the damage entity that is at our location
				auto view = data.m_reg.view<ComponentView::damageDone, ComponentView::mapObject, ComponentView::position>();
				for (auto spikeEntity : view) {
					auto& spikePos = view.get<ComponentView::position>(spikeEntity);

					// if were in the same place
					if (pos.curx() == spikePos.curx() && pos.cury() == spikePos.cury()) {
						//std::cout << "Found the spike in our position, updating the damage done" << std::endl;
						// Decrement the players health by the damage caused by the spikes
						health.setHp(health.hp() - view.get<ComponentView::damageDone>(spikeEntity).damage);
						// remove the spike entity
					//	std::cout << "removing the spike entity" << std::endl;	// I think the bug is with remove entitiy, or with the fact
																				// that were unlocking after removing an entity......
					//	NetworkUtilitySystem::removeEntity(data, nullptr, TranslationSystem::getNetId(data, spikeEntity), true, false);

					}
				}
				health.unlock(data, entity);
			}
		}
		
	}*/
}
