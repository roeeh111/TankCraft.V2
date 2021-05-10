#include "CoinMob.h"
#include "Components.h"
#include "IDTranslationSystem.h"
#include "NetworkUtilitySystem.h"

namespace Coins {
	void addCoins(GameData::GameData& data, RakNet::Packet* pack, uint32_t points, uint32_t x, uint32_t y)
	{
		auto clientEntity = NetworkUtilitySystem::addEntity(data, pack, true, true);
		//std::cout << "Adding coins for entity" << TranslationSystem::getNetId(data, clientEntity) << std::endl;

		// Add the components to the registry
		(data.m_reg.emplace<ComponentView::mapObject>(clientEntity, 'C', true)).unlock(data, clientEntity);
		(data.m_reg.emplace<ComponentView::position>(clientEntity, true, x, y)).unlock(data, clientEntity);
		(data.m_reg.emplace<ComponentView::pointsGiven>(clientEntity, points)).unlock(data, clientEntity);
	}
}
