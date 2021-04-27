#include "ReflectionSystem.h"

namespace  ReflectionSystem {

	void UpdatePacket::writeHeader(std::list<baseComponent*> components)
	{
		// Loop through all components, and write the id's to the header
		for (auto& comp : components) {
			std::cout << "Pushing back id = " << comp->CompId << std::endl;
			header.ids.push_back(comp->CompId);
		}

		// Set written header = true so that we can now write the header. Also write the header
		Serialize<UpdatePacketHeader>(header);							// TODO: we get errors on this line...
	}


	// TODO::::!!!!!!	HOW ARE WE WRITING THE COMPONENTS!!!!!!!!!!!
	void UpdatePacket::writeComponents(std::list<baseComponent*> components)
	{
		for (auto& comp : components) {
			comp->Serialize(sbuf);				//// THIS IS GONNA SERIALIZE A BASE COMPONENT??????..... SO FIGURE OUT HOW WE WANT TO SPECIFY COMPONENTS!!
											//		(can have each decleration of msgpackdefine create a serialization function via autogeneration)
		}
	}

	msgpack::sbuffer& UpdatePacket::Serialize(std::list<baseComponent*> components)
	{
		writeHeader(components);
		writeComponents(components);
		return sbuf;
	}


	// TODO::: NEED TO MAKE CHANGES TO REFLECT WHAT THE DEBUG FUNCTION LOOKS LIKE!
	void ReflectionSystem::MakeGameUpdate(GameData::GameData& data, std::string& stream)
	{
		// deserialize these objects using msgpack::unpacker.
		msgpack::unpacker pac;

		// feed the buffer.
		pac.reserve_buffer(stream.size());
		memcpy(pac.buffer(), stream.data(), stream.size());
		pac.buffer_consumed(stream.size());



		// now starts streaming deserialization.
		msgpack::object_handle oh;
		msgpack::object obj;

		// The first element will always be the header, so get it
		UpdatePacketHeader header;
		pac.next(oh);

		// check if the entity with netid exists in the game, if not, add it to the registry (if we do this, then this is essentially an add entity packet....)
		if (!TranslationSystem::hasMapping(data, header.netid)) {
			networkID setMapping(GameData::GameData & data, networkID netId, entt::entity entityId);
		}

		auto enttid = TranslationSystem::getEntity(data, header.netid);


		int i = 0;
		while (pac.next(oh)) {
			// Call component write function based on the switch statement

			MakeGameUpdateHelper(data, oh.get(), header.ids[i], enttid);
			i++;
		}
	}


	//							TODO: PASS IN THE ENTT ID OF THE ENTITY, NOT THE NETIDS
	// given an object and it component id find the appropriate type of object, and write it to the register
	void ReflectionSystem::MakeGameUpdateHelper(GameData::GameData& data, const msgpack::object& obj, ComponentID::ComponentID id, entt::entity& enttid)
	{
		switch (id) {
		case ComponentID::Position:
			writeComponent<ComponentView::position>(data, obj, enttid);
			std::cout << "writing a component!" << std::endl;
			break;
		case ComponentID::MapObject:
			writeComponent<ComponentView::mapObject>(data, obj, enttid);
			std::cout << "writing a component!" << std::endl;
			break;
		case ComponentID::Score:
			//writeComponent<ComponentView::score>(data, obj, enttid);
			std::cout << "writing a component!" << std::endl;
			break;
		case ComponentID::ClientName:
		//	writeComponent<ComponentView::clientName>(data, obj, enttid);
			break;
		case ComponentID::Health:
		//	writeComponent<ComponentView::health>(data, obj, enttid);
			break;
		case ComponentID::input:
		//	writeComponent<ComponentView::userInput>(data, obj, enttid);
			break;

		default:
			std::cout << "do nothing for now" << std::endl;
			break;
		}
	}

	// DEBUG ONLY
	void ReflectionSystem::MakeGameUpdate(GameData::GameData& data, msgpack::sbuffer& stream)
	{
		// deserialize these objects using msgpack::unpacker.
		msgpack::unpacker pac;

		// feed the buffer.
		pac.reserve_buffer(stream.size());
		memcpy(pac.buffer(), stream.data(), stream.size());
		pac.buffer_consumed(stream.size());

		// now starts streaming deserialization.
		msgpack::object_handle oh;
		msgpack::object obj;

		// The first element will always be the header, so get it
		UpdatePacketHeader header;			

		// The entity id inside the header.
		entt::entity enttid;

		int i = 0;
		while (pac.next(oh)) {
			// Call component write function based on the switch statement
			if (i == 0) {
				oh.get().convert(header);

				// check if the entity with netid exists in the game, if not, add it to the registry (if we do this, then this is essentially an add entity packet....)
				if (!TranslationSystem::hasMapping(data, header.netid)) {
					networkID setMapping(GameData::GameData & data, networkID netId, entt::entity entityId);
				}

				enttid = TranslationSystem::getEntity(data, header.netid);
			}
			else {
				MakeGameUpdateHelper(data, oh.get(), header.ids[i-1], enttid);
			}
			i++;
		}
	}







}

/*
// Check if component exists already in the registry, if yes, fill it with the values in the msgpack object
entt::entity enttid = TranslationSystem::getEntity(data, netid);

if (!data.m_reg.has<ComponentView::position>(enttid)) {
	auto enttobj = data.m_reg.emplace<ComponentView::position>(enttid);
	obj.convert(enttobj);
}
else {
	auto enttobj = data.m_reg.get<ComponentView::position>(enttid);
	obj.convert(enttobj);
}

*/