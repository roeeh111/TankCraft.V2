#include "Tank.h"
#include "NetworkUtilitySystem.h"


void Tank::addTank(GameData::GameData& data, RakNet::Packet* pack, std::string& loginName)
{
	auto clientEntity = NetworkUtilitySystem::addEntity(data, pack, true, true);				// OK WTF, now the compid is wrong, and were crashing on adding an entitty
	std::cout << "Adding tank for entity" << TranslationSystem::getNetId(data, clientEntity) << std::endl;

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


/*

OK for some reason, if i call addspikes after calling addtank, the compid doesnt register?? and the server crashes.... So wtf... fix it
*/