#include "ReflectionSystem.h"
#include "NetworkUtilitySystem.h"
#include <RakNetTypes.h>
#include <BitStream.h>
#include <MessageIdentifiers.h>

namespace  ReflectionSystem {
	
	// DEPRECATED
	void UpdatePacket::writeHeader(std::list<baseComponent*> components)
	{
		// Loop through all components, and write the id's to the header (remember that i changed to pointer)
		for (auto* comp : components) {
			std::cout << "Pushing back id = " << comp->CompId << "Comp internals = " <<std::endl;				// ERRORR THE ID IS WRONG WHEN WERE TRYING TO GET IT
			comp->print();
			header.ids.push_back(comp->CompId);
		}

		// Set written header = true so that we can now write the header. Also write the header
		Serialize<UpdatePacketHeader::UpdatePacketHeader>(header);							// TODO: we get errors on this line...
	}

	// DEPRECATED
	void UpdatePacket::writeComponents(std::list<baseComponent*> components)
	{
		for (auto& comp : components) {
			std::cout << "Writing component with compid = :" << comp->CompId << std::endl;;
			comp->print();
			comp->Serialize(sbuf);	
		}
	}


	// DEPRECATED
	msgpack::sbuffer& UpdatePacket::Serialize(std::list<baseComponent*> components)
	{
		std::cout << "in serialize, writing the header" << std::endl;
		writeHeader(components);														// No need to write the header
		std::cout << "in serialize, DONE writing the header, moving on to the components" << std::endl;
		writeComponents(components);
		std::cout << "in serialize, DONE writing the components" << std::endl;
		return sbuf;
	}



	msgpack::sbuffer& UpdatePacket::Serialize(GameData::GameData& data)
	{

		// First, serialize the header into the update packet
		Serialize<UpdatePacketHeader::UpdatePacketHeader>(header);

		// next loop through the header and for each compid in the header, serialize it into the update packet
		for (auto compid : header.ids) {
			// Call serialize with netid, headerid
			SerializeComponent(data, TranslationSystem::getEntity(data, header.netid), compid);
		}
		return sbuf;
	}

	void UpdatePacket::SerializeComponent(GameData::GameData& data, entt::entity id, ComponentID::ComponentID compid)
	{
		switch (compid) {
		case ComponentID::Position:
			if (data.m_reg.has<ComponentView::position>(id)) 
				Serialize<ComponentView::position>(data.m_reg.get<ComponentView::position>(id));
			//	data.m_reg.get<ComponentView::position>(id).print();

			break;
		case ComponentID::MapObject:
			if (data.m_reg.has<ComponentView::mapObject>(id))
				Serialize<ComponentView::mapObject>(data.m_reg.get<ComponentView::mapObject>(id));
		//	data.m_reg.get<ComponentView::mapObject>(id).print();

			break;
		case ComponentID::Score:
			if (data.m_reg.has<ComponentView::score>(id))
				Serialize<ComponentView::score>(data.m_reg.get<ComponentView::score>(id));
		//	data.m_reg.get<ComponentView::score>(id).print();

			break;
		case ComponentID::ClientName:
			if (data.m_reg.has<ComponentView::clientName>(id))
				Serialize<ComponentView::clientName>(data.m_reg.get<ComponentView::clientName>(id));
			//data.m_reg.get<ComponentView::clientName>(id).print();

			break;
		case ComponentID::Health:
			if (data.m_reg.has<ComponentView::health>(id))
				Serialize<ComponentView::health>(data.m_reg.get<ComponentView::health>(id));
			break;
		case ComponentID::input:
			if (data.m_reg.has<ComponentView::userInput>(id))
				Serialize<ComponentView::userInput>(data.m_reg.get<ComponentView::userInput>(id));
			break;
		case ComponentID::MapComponent:
			if (data.m_reg.has<ComponentView::MapComponent>(id))
				Serialize<ComponentView::MapComponent>(data.m_reg.get<ComponentView::MapComponent>(id));
			break;

		case ComponentID::DamageDone:
			if (data.m_reg.has<ComponentView::damageDone>(id))
				Serialize<ComponentView::damageDone>(data.m_reg.get<ComponentView::damageDone>(id));
			break;

		case ComponentID::PointsGiven:
			if (data.m_reg.has<ComponentView::pointsGiven>(id))
				Serialize<ComponentView::pointsGiven>(data.m_reg.get<ComponentView::pointsGiven>(id));
			break;

		case ComponentID::Velocity:
			if (data.m_reg.has<ComponentView::velocity>(id))
				Serialize<ComponentView::velocity>(data.m_reg.get<ComponentView::velocity>(id));
			break;

		default:
			std::cout << "do nothing for now" << std::endl;
			break;
		}
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
		UpdatePacketHeader::UpdatePacketHeader header;

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
			//std::cout << "After component is written" << std::endl;
		//	std::cout << "writing a component!" << std::endl;
			break;
		case ComponentID::MapObject:
			writeComponent<ComponentView::mapObject>(data, obj, enttid);
		//	std::cout << "writing a component!" << std::endl;
			break;
		case ComponentID::Score:
			writeComponent<ComponentView::score>(data, obj, enttid);
		//	std::cout << "writing a component!" << std::endl;
			break;
		case ComponentID::ClientName:
			writeComponent<ComponentView::clientName>(data, obj, enttid);
		//	std::cout << "writing a component!" << std::endl;
			break;
		case ComponentID::Health:
			writeComponent<ComponentView::health>(data, obj, enttid);
			break;
		case ComponentID::input:
			writeComponent<ComponentView::userInput>(data, obj, enttid);
		//	std::cout << "writing a component!" << std::endl;
			break;
		case ComponentID::MapComponent:
		//	std::cout << "writing a component!" << std::endl;
			writeComponent<ComponentView::MapComponent>(data, obj, enttid);
			break;

		case ComponentID::DamageDone:
			writeComponent<ComponentView::damageDone>(data, obj, enttid);
			break;

		case ComponentID::PointsGiven:
			writeComponent<ComponentView::pointsGiven>(data, obj, enttid);
			break;

		case ComponentID::Velocity:
			writeComponent<ComponentView::velocity>(data, obj, enttid);
			break;

		default:
			std::cout << "do nothing for now" << std::endl;
			break;
		}
	}

	void ReflectionSystem::update(GameData::GameData& data)
	{
		if (data.compUpdateMap.size() <= 0)
			return;
		//std::cout << "Flushing game update of size " << data.updateMap.size() << std::endl;


		// Loop over all mappings in the map, and write their serialized packets to the stream
		for (auto& it : data.compUpdateMap) {
	//		std::cout << "flushing update for id " << it.first << std::endl;
			RakNet::BitStream stream = RakNet::BitStream();

			// write the packet type to the bitsream
			RakNet::MessageID type = UPDATE_ENTITY;
			stream.Write((char*)&type, sizeof(RakNet::MessageID));

	//		std::cout << "wrote id to the bitstream"  << std::endl;

			// Serialize the game update
			UpdatePacket gameUpdate(it.second);

			// TODO: Change this to only call write components on the header! (write components should also be changed)
			msgpack::sbuffer& buf = gameUpdate.Serialize(data);			// ERROR IN THIS LINE OF CODE!!! (IDK BUT SOMETHING AINT SERIALIZING)
		//	std::cout << "Serialized the game update object" << std::endl;

		//	std::cout << "buffer size for game update = " << buf.size() << std::endl;

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
	//	std::cout << "Finished flushing the map" << std::endl;

		// Lastly, clear the update map now that weve done all of the updates
		data.updateMap.clear();
		data.compUpdateMap.clear();
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
		UpdatePacketHeader::UpdatePacketHeader header;

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


/* Previous update function
		if (data.updateMap.size() <= 0)
			return;
		//std::cout << "Flushing game update of size " << data.updateMap.size() << std::endl;

		//DEBUG: print the map
		std::cout << "in update, printing map:" << std::endl;
		for (auto& it : data.updateMap) {
			std::cout << "netid = " << it.first << std::endl;
			for (auto* comp : it.second) {
				comp->print();
			}
		}
		std::cout << "end of map" << std::endl;

		// Loop over all mappings in the map, and write their serialized packets to the stream
		for (auto& it : data.updateMap) {
			std::cout << "flushing update for id " << it.first << std::endl;
			RakNet::BitStream stream = RakNet::BitStream();

			// write the packet type to the bitsream
			RakNet::MessageID type = UPDATE_ENTITY;
			stream.Write((char*)&type, sizeof(RakNet::MessageID));

			std::cout << "wrote id to the bitstream"  << std::endl;

			// Serialize the game update
			UpdatePacket gameUpdate(it.first);

			// DEBUG:
			std::cout << "created the game update object for netid = " << it.first << std::endl;
			std::cout << "List of components = " << std::endl;
			for (auto* comp : it.second) {
				comp->print();
			}

			msgpack::sbuffer& buf = gameUpdate.Serialize(it.second);			// ERROR IN THIS LINE OF CODE!!! (IDK BUT SOMETHING AINT SERIALIZING)
			std::cout << "Serialized the game update object" << std::endl;

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





*/