#pragma once
#include <entt/entt.hpp>
#include "GameData.h"
#include "FreeListComponent.h"
#include "IDTranslationComponent.h"
#include "Tanks.pb.h"


namespace TranslationSystem {
	class IDTranslation {
	public:
		// Set this specific id as free
		void freeID(GameData::GameData& data, networkID id);
		
		// Create a mapping of new netid->entityid
		networkID createMapping(GameData::GameData& data, entt::entity entityId);

		networkID setMapping(GameData::GameData& data, networkID netId, entt::entity entityId);

		void addEntity(GameData::GameData& data, ProtoMessaging::AddRemoveEntityMessage* msg);

		void removeEntity(GameData::GameData& data, ProtoMessaging::AddRemoveEntityMessage* msg);

		// Get the entity associated with the netid
		entt::entity getEntity(GameData::GameData& data, networkID netId);

		// Whether the system has a mapping for this netId
		bool hasMapping(SceneComponent::SceneComponent& data, networkID netId);

	private:
		networkID allocateID(entt::registry& m_reg);
		FreeListComponent::freelist &getFreelist(entt::registry& m_reg);

	};

}

/*
*
* System for translating network id to entity id and generating new network id's
*
* Pragmas:
* -Server will generate a network id for an entity upon entitiy creation
* -Server will keep its own mapping of network id to entity id
* -After generation, Server will send to the client to generate a new entity, and add a new mapping of the server's networkid->entityid
* -System will map an networkID to its local entityID (each client/server has different entity ids/different mappings. but the same networkids)
*
*
* Underlying structure:
* - a map of long to entt:entity
* - Free list of availible values (array of booleans of size max uint32_t. Use the linux method to get free elements)
* - GenerateID() -> Generates a new network id to be used
* - FreeID() -> sets an id to be usable (call this on entitiy remove)
* - CreateMapping() -> generates a new id, and maps that id to the local entity
* - IDMap is stored globally in the scene
*
* To get the entityid associated with this netid, just call get with the map
*/

