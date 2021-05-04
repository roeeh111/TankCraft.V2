#include "ZombieMob.h"
#include "NetworkUtilitySystem.h"

namespace Zombie {
	void addZombie(GameData::GameData& data, RakNet::Packet* pack, uint32_t damageDone, uint32_t health, uint32_t dx, uint32_t dy)
	{
		auto clientEntity = NetworkUtilitySystem::addEntity(data, pack, true, true);
		std::cout << "Adding zombie for entity" << TranslationSystem::getNetId(data, clientEntity) << std::endl;

		// Add the components to the the registry
		(data.m_reg.emplace<ComponentView::mapObject>(clientEntity, 'Z', true)).unlock(data, clientEntity);
		(data.m_reg.emplace<ComponentView::position>(clientEntity, true, 1, 1)).unlock(data, clientEntity);
		(data.m_reg.emplace<ComponentView::damageDone>(clientEntity, damageDone)).unlock(data, clientEntity);
		(data.m_reg.emplace<ComponentView::health>(clientEntity, health, true)).unlock(data, clientEntity);
		(data.m_reg.emplace<ComponentView::velocity>(clientEntity, dx, dy)).unlock(data, clientEntity);
	}
}
