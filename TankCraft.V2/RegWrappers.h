#pragma once
#include <entt/entt.hpp>
#include "Components.h"
#include "IDTranslationSystem.h"
#include "ReflectionSystem.h"

/*
* Wrapper of entity registry.create(), gives the option to label the entity as networked by adding a network component
*/

namespace RegWrapper{

	// Create an entity in the m_reg. If networked is set, add a network component to signify that it should be networked
	entt::entity createEntity(entt::registry& m_reg, bool networked);

	template <typename Type>
	void enttRemove(GameData::GameData& data, entt::entity entity) {

		// First check if register has the component
		if (!data.m_reg.has<Type>(entity))
			return;

		auto netid = TranslationSystem::getNetId(data, entity);

		// If this netid is already in the list, 
		if (data.compUpdateMap.count(-netid) == 0) {
			data.compUpdateMap[-netid] = UpdatePacketHeader::UpdatePacketHeader(-netid);
		}

		data.compUpdateMap[-netid].ids.insert(data.m_reg.get<Type>(entity).CompId);

		//	TODO: REMOVE THE COMPONENT FROM THE UPDATE LOG
		ReflectionSystem::RemoveLogComponent(data, data.m_reg.get<Type>(entity).CompId);


		data.m_reg.remove_if_exists<Type>(entity);
	}
}