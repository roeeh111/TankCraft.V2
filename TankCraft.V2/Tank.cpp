#include "Tank.h"
#include "NetworkUtilitySystem.h"


void Tank::addTank(GameData::GameData& data, RakNet::Packet* pack, std::string& loginName)
{
	auto clientEntity = NetworkUtilitySystem::addEntity(data, pack, true, true);				// OK WTF, now the compid is wrong, and were crashing on adding an entitty
	std::cout << "Adding tank for entity" << TranslationSystem::getNetId(data, clientEntity) <<  " With login name = " << loginName <<std::endl;

	// Add the components to the the registry
	(data.m_reg.emplace<ComponentView::mapObject>(clientEntity, true)).unlock(data, clientEntity);			// NOTE: ok this is fucked for some reason, these guys are assholes
	(data.m_reg.emplace<ComponentView::position>(clientEntity, true, true)).unlock(data, clientEntity);
	(data.m_reg.emplace<ComponentView::clientName>(clientEntity, loginName, true)).unlock(data, clientEntity);
	(data.m_reg.emplace<ComponentView::userInput>(clientEntity, true)).unlock(data, clientEntity);
	(data.m_reg.emplace<ComponentView::score>(clientEntity, true)).unlock(data, clientEntity);
	(data.m_reg.emplace<ComponentView::health>(clientEntity, true)).unlock(data, clientEntity);

	//std::cout << "Added tank for " << loginName << std::endl;
	//std::cout << "In registry: " << data.m_reg.has<ComponentView::mapObject, ComponentView::userInput, ComponentView::score>(clientEntity) << std::endl;
	// DEBUG: print out the map
	//for (auto* comp : data.updateMap[TranslationSystem::getNetId(data, clientEntity)]) {
	//	comp->print();
//	}
//	std::cout << std::endl;
}

// Create a new tank entity, and add it to the updatemap/log
void Tank::logTank(GameData::GameData& data, RakNet::Packet* pack, std::string& loginName) {

	auto newEntity = data.m_reg.create();

	// Allocate a new netId for this entity (make sure that this entity doesnt already exist...)
	networkID netid = TranslationSystem::createMapping(data, newEntity);

	// If the client map isnt constructed for this address, construct it
	if (!data.clientAddressToEntities.count(pack->systemAddress)) {
		data.clientAddressToEntities[pack->systemAddress] = std::list<networkID>();
	}

	// Append into the client entity map
	data.clientAddressToEntities[pack->systemAddress].push_back(netid);

	(data.m_reg.emplace<ComponentView::mapObject>(newEntity, true)).unlock(data, newEntity);			// NOTE: ok this is fucked for some reason, these guys are assholes
	(data.m_reg.emplace<ComponentView::position>(newEntity, true, true)).unlock(data, newEntity);
	(data.m_reg.emplace<ComponentView::clientName>(newEntity, loginName, true)).unlock(data, newEntity);
	(data.m_reg.emplace<ComponentView::userInput>(newEntity, true)).unlock(data, newEntity);
	(data.m_reg.emplace<ComponentView::score>(newEntity, true)).unlock(data, newEntity);
	(data.m_reg.emplace<ComponentView::health>(newEntity, true)).unlock(data, newEntity);


}
