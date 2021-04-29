#include "ReflectionSystem.h"
#include "NetworkUtilitySystem.h"
#include <RakNetTypes.h>
#include <BitStream.h>
#include <MessageIdentifiers.h>

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


	void UpdatePacket::writeComponents(std::list<baseComponent*> components)
	{
		for (auto& comp : components) {
			comp->Serialize(sbuf);	
		}
	}

	msgpack::sbuffer& UpdatePacket::Serialize(std::list<baseComponent*> components)
	{
		writeHeader(components);
		writeComponents(components);
		return sbuf;
	}


	void ReflectionSystem::MakeGameUpdate(GameData::GameData& data, std::string& stream)
	{
	//	std::cout << "in reflection, making a game update, size of buffer =  " << stream.size() << std::endl;
		// deserialize these objects using msgpack::unpacker.
		msgpack::unpacker pac;

		// feed the buffer.
		pac.reserve_buffer(stream.size());
		memcpy(pac.buffer(), stream.data(), stream.size());
		pac.buffer_consumed(stream.size());

		// now starts streaming deserialization.
		msgpack::object_handle oh;

		// The first element will always be the header, so get it
		UpdatePacketHeader header;

		// The entity id inside the header.
		entt::entity enttid;

		// Begin i at -1, so we can read from the header and not have to offset indexing into the header's component ids
		int i = -1;
		while (pac.next(oh)) {
			// Call component write function based on the switch statement
			if (i == -1) {
		//		std::cout << "Converting header" << std::endl;
				oh.get().convert(header);
		//		std::cout << "header netid = " << header.netid << std::endl;
		//		std::cout << "header # of updates = " << header.ids.size() <<  std::endl;

				// check if the entity with netid exists in the game, if not, add it to the registry (if we do this, then this is essentially an add entity packet....)
				if (!TranslationSystem::hasMapping(data, header.netid)) {
					networkID setMapping(GameData::GameData & data, networkID netId, entt::entity entityId);
				}

				enttid = TranslationSystem::getEntity(data, header.netid);
			}
			else {
				MakeGameUpdateHelper(data, oh.get(), header.ids[i], enttid);
			}
			i++;
		}
	}


	// given an object and it component id find the appropriate type of object, and write it to the register
	void ReflectionSystem::MakeGameUpdateHelper(GameData::GameData& data, const msgpack::object& obj, ComponentID::ComponentID id, entt::entity& enttid)
	{
		switch (id) {
		case ComponentID::Position:
			writeComponent<ComponentView::position>(data, obj, enttid);
		//	std::cout << "writing a component!" << std::endl;
			break;
		case ComponentID::MapObject:
			writeComponent<ComponentView::mapObject>(data, obj, enttid);
		//	std::cout << "writing a component!" << std::endl;
			break;
		case ComponentID::Score:
			//writeComponent<ComponentView::score>(data, obj, enttid);
		//	std::cout << "writing a component!" << std::endl;
			break;
		case ComponentID::ClientName:
			writeComponent<ComponentView::clientName>(data, obj, enttid);
		//	std::cout << "writing a component!" << std::endl;
			break;
		case ComponentID::Health:
		//	writeComponent<ComponentView::health>(data, obj, enttid);
			break;
		case ComponentID::input:
			writeComponent<ComponentView::userInput>(data, obj, enttid);
		//	std::cout << "writing a component!" << std::endl;
			break;
		case ComponentID::MapComponent:
		//	std::cout << "writing a component!" << std::endl;
			writeComponent<ComponentView::MapComponent>(data, obj, enttid);
			break;

		default:
			std::cout << "do nothing for now" << std::endl;
			break;
		}
	}

	void ReflectionSystem::update(GameData::GameData& data)
	{
		if (data.updateMap.size() <= 0)
			return;
		//std::cout << "Flushing game update of size " << data.updateMap.size() << std::endl;

		// Loop over all mappings in the map, and write their serialized packets to the stream
		for (auto& it : data.updateMap) {
			std::cout << "flushing update for id " << it.first << std::endl;
			RakNet::BitStream stream = RakNet::BitStream();

			// write the packet type to the bitsream
			RakNet::MessageID type = UPDATE_ENTITY;
			stream.Write((char*)&type, sizeof(RakNet::MessageID));

			// Serialize the game update
			UpdatePacket gameUpdate(it.first);
			msgpack::sbuffer& buf = gameUpdate.Serialize(it.second);

			std::cout << "buffer size for game update = " << buf.size() << std::endl;

			// Write the game update to the stream, and broadast it to all connections
			stream.Write(buf.data(), buf.size());

		//	NetworkUtilitySystem::broadcast(data, &stream, HIGH_PRIORITY, RELIABLE_ORDERED, 0);

			data.rpi->Send(&stream,
				HIGH_PRIORITY,
				RELIABLE_ORDERED,
				0,
				RakNet::UNASSIGNED_SYSTEM_ADDRESS,
				true);

			// DEBUG:
		/*	msgpack::unpacker pac;

			// feed the buffer.
			pac.reserve_buffer(buf.size());
			memcpy(pac.buffer(), buf.data(), buf.size());
			pac.buffer_consumed(buf.size());

			// now starts streaming deserialization.
			msgpack::object_handle oh;

			// The first element will always be the header, so get it

			// The entity id inside the header.

			int i = -1;
			std::cout << "DEBUG REFLECTION UPDATE: PRINTING THE UPDATE:" << std::endl;
			while (pac.next(oh)) {
				// Call component write function based on the switch statement
				std::cout << oh.get() << std::endl;
			}
			*/

		}

		// Lastly, clear the update map now that weve done all of the updates
		data.updateMap.clear();
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