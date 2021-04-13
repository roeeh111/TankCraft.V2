#include "GameData.h"
#include "IDTranslationSystem.h"

void baseComponent_::unlock(GameData::GameData& data, const entt::entity& entity)
{
	//std::cout << "calling unlock" << std::endl;
	// TODO: actually unlock the component

	// If the component is networked, append the component to the map
	if (networked) {
		// If the component is already in the update map, remove it and add this one	// TODO:: this is a big design decision, talk with team about it 
		// (keep the most rescent changes to the component)
	//	std::cout << "IN UNLOCK, ADDING THE COMPONENT TO THE UPDATE MAP" << std::endl;
		data.updateMap[TranslationSystem::getNetId(data, entity)].push_back(this);
	}
}

