#include "ReflectionSystem.h"
#include "NetworkUtilitySystem.h"
#include <RakNetTypes.h>
#include <BitStream.h>
#include <MessageIdentifiers.h>

namespace  ReflectionSystem {
	

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

			// Serialize the game update
			UpdatePacket gameUpdate(it.second);
			msgpack::sbuffer& buf = gameUpdate.Serialize(data);	


			// Write the game update to the stream, and broadast it to all connections
			stream.Write(buf.data(), buf.size());

			//	NetworkUtilitySystem::broadcast(data, &stream, HIGH_PRIORITY, RELIABLE_ORDERED, 0);

			data.rpi->Send(&stream,
				HIGH_PRIORITY,
				RELIABLE_ORDERED,
				0,
				RakNet::UNASSIGNED_SYSTEM_ADDRESS,
				true);

			UpdateLog(data, it.second);


		}
		// Lastly, clear the update map now that weve done all of the updates
		data.compUpdateMap.clear();
	}



	msgpack::sbuffer& UpdatePacket::Serialize(GameData::GameData& data)
	{

		// First, serialize the header into the update packet
		Serialize<UpdatePacketHeader::UpdatePacketHeader>(header);

		// next loop through the header and for each compid in the header, serialize it into the update packet
		// Only if the netid is encoded as posotive
		if (header.netid >= 0) {							// NEW, might not work
			for (auto compid : header.ids) {
				// Call serialize with netid, headerid
				SerializeComponent(data, TranslationSystem::getEntity(data, header.netid), compid);
			}
		}
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
		UpdatePacketHeader::UpdatePacketHeader header;

		// The entity id inside the header.
		entt::entity enttid;

		// Begin i at -1, so we can read from the header and not have to offset indexing into the header's component ids
		int i = -1;
		std::set<ComponentID::ComponentID>::iterator it;

		while (pac.next(oh)) {
			// Call component write function based on the switch statement
			if (i == -1) {

				// Deserialize the header component into the header
				oh.get().convert(header);

				// Check if the netid is negetive, if yes its a remove component packet, so call removePacketHelper and return
				if (header.netid < 0) {
					RemoveComponents(data, header);
					return;
				}

				// check if the entity with netid exists in the game, if not, add it to the registry (if we do this, then this is essentially an add entity packet....)
				if (!TranslationSystem::hasMapping(data, header.netid)) {
					//std::cout << "In make update... netid " << header.netid << " Does not have a mapping, creating a new mapping" << std::endl;
					enttid = data.m_reg.create();

					TranslationSystem::setMapping(data, header.netid, enttid);
				}
				else {
					enttid = TranslationSystem::getEntity(data, header.netid);
				}


				// Set the id iterator as well
				it = header.ids.begin();
			}
			else {
				if (it != header.ids.end()) {
					MakeGameUpdateHelper(data, oh.get(), *it, enttid);
					it++;
				}
			}
			i++;
		}
	}

	void ReflectionSystem::RemoveComponents(GameData::GameData& data, UpdatePacketHeader::UpdatePacketHeader& header)
	{
		auto entity = TranslationSystem::getEntity(data, -header.netid);	// TODO MAKE SUR3E THE ENTITY IS IN THE GAME
		for (auto& compid : header.ids) {
			RemoveComponentsHelper(data, entity, compid);
		}

	}

	void ReflectionSystem::RemoveComponentsHelper(GameData::GameData& data, entt::entity id, ComponentID::ComponentID compid)
	{
		switch (compid) {
		case ComponentID::Position: {
			data.m_reg.remove_if_exists<ComponentView::position>(id);
			break;
		}

		case ComponentID::MapObject: {
			data.m_reg.remove_if_exists<ComponentView::mapObject>(id);
			break;
		}

		case ComponentID::Score: {
			data.m_reg.remove_if_exists<ComponentView::score>(id);
			break;
		}

		case ComponentID::ClientName: {
			data.m_reg.remove_if_exists<ComponentView::clientName>(id);
			break;
		}

		case ComponentID::Health: {
			data.m_reg.remove_if_exists<ComponentView::health>(id);
			break;
		}

		case ComponentID::input: {
			data.m_reg.remove_if_exists<ComponentView::userInput>(id);
			break;
		}

		case ComponentID::MapComponent: {
			data.m_reg.remove_if_exists<ComponentView::MapComponent>(id);
			break;
		}

		case ComponentID::DamageDone: {
			data.m_reg.remove_if_exists<ComponentView::damageDone>(id);
			break;
		}
			
		case ComponentID::PointsGiven: {
			data.m_reg.remove_if_exists<ComponentView::pointsGiven>(id);
			break;
		}

		case ComponentID::Velocity: {
			data.m_reg.remove_if_exists<ComponentView::velocity>(id);
			break;
		}

		default: {
			std::cout << "do nothing for now" << std::endl;
			break;
		}
		}
	}



	// given an object and it component id find the appropriate type of object, and write it to the register
	void ReflectionSystem::MakeGameUpdateHelper(GameData::GameData& data, const msgpack::object& obj, ComponentID::ComponentID id, entt::entity& enttid)

	{
		switch (id) {
		case ComponentID::Position:			// PRIOR TO THIS, WE KEEP CREATING A NEW NETID-ENTTTID PAIRING IN PLAYER 2
		//	std::cout << "Updating position for netid " << TranslationSystem::getNetId(data, enttid)  << " With entt id = " << (int) enttid << std::endl;
			writeComponent<ComponentView::position>(data, obj, enttid);
			break;

		case ComponentID::MapObject:
			writeComponent<ComponentView::mapObject>(data, obj, enttid);
			break;

		case ComponentID::Score:
			writeComponent<ComponentView::score>(data, obj, enttid);
			break;

		case ComponentID::ClientName:
			writeComponent<ComponentView::clientName>(data, obj, enttid);
			break;

		case ComponentID::Health:
			writeComponent<ComponentView::health>(data, obj, enttid);
			break;

		case ComponentID::input:
			writeComponent<ComponentView::userInput>(data, obj, enttid);
			break;

		case ComponentID::MapComponent:
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


	void UpdatePacket::SerializeComponent(GameData::GameData& data, entt::entity id, ComponentID::ComponentID compid)
	{
		switch (compid) {
		case ComponentID::Position:
			if (data.m_reg.has<ComponentView::position>(id)) {
				Serialize<ComponentView::position>(data.m_reg.get<ComponentView::position>(id));
				//std::cout << "Serializing position for netid " << TranslationSystem::getNetId(data, id) << " With entt id = " << (int) id << std::endl;
			}

			break;
		case ComponentID::MapObject:
			if (data.m_reg.has<ComponentView::mapObject>(id))
				Serialize<ComponentView::mapObject>(data.m_reg.get<ComponentView::mapObject>(id));

			break;
		case ComponentID::Score:
			if (data.m_reg.has<ComponentView::score>(id))
				Serialize<ComponentView::score>(data.m_reg.get<ComponentView::score>(id));

			break;
		case ComponentID::ClientName:
			if (data.m_reg.has<ComponentView::clientName>(id))
				Serialize<ComponentView::clientName>(data.m_reg.get<ComponentView::clientName>(id));

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



	void UpdateLog(GameData::GameData& data, UpdatePacketHeader::UpdatePacketHeader header)
	{
		// Check if there is already a log for this packet
		if (data.compLog.count(header.netid) == 0) {
			data.compLog[header.netid] = header;
		}
		else {
			// If the netid is already in the update log, add whatever is in the update header
			for (auto& compid : header.ids)
				data.compLog[header.netid].ids.insert(compid);
		}
	}
}
