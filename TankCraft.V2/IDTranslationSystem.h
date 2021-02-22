#pragma once
#include <cstdint>
#include <entt/entt.hpp>

typedef uint32_t networkID;

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
*Underlying structure:
* - a map of long to entt:entity
* - GenerateID() -> Generates a new network id to be used
* - FreeID() -> sets an id to be usable (call this on entitiy remove)
* - CreateMapping() -> generates a new id, and maps that id to the local entity
* - IDMap is stored globally in the scene
*
*/

namespace TranslationSystem {
	class IDTranslation {
	public:
		// Set this specific id as free
		void freeID(networkID id);
		
		// Create a mapping of new netid->entityid
		networkID createMapping(entt::entity entityId);

		// Get the entityid associated with this netid
		entt::entity get(networkID netId);
	private:
		networkID generateID(entt::entity entityId);

	};
}

