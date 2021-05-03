#pragma once
#include <msgpack.hpp>
#include <vector>
#include "GameData.h"
#include "ComponentID.h"
#include "Components.h"
#include "IDTranslationComponent.h"
#include "IDTranslationSystem.h"
#include "UpdatePacketHeader.h"


namespace ReflectionSystem {




	class UpdatePacket {
	public:

		UpdatePacket(networkID netid_) {
			header = UpdatePacketHeader::UpdatePacketHeader(netid_);
			pk = new msgpack::packer<msgpack::sbuffer>(&sbuf);
		}
		
		UpdatePacket(UpdatePacketHeader::UpdatePacketHeader header_) {
			header = header_;
			pk = new msgpack::packer<msgpack::sbuffer>(&sbuf);
		}
		
		~UpdatePacket() { delete pk; };
		
		msgpack::sbuffer& Serialize(std::list<baseComponent*> components);			// DEPRICATED

		// Serialize the update packet into a buffer
		msgpack::sbuffer& Serialize(GameData::GameData &data);
		// Serialize a component into theclass buffer
	private:
		msgpack::sbuffer sbuf;
		msgpack::packer<msgpack::sbuffer>* pk;
		UpdatePacketHeader::UpdatePacketHeader header;

		// Write all component id's into the header
		void writeHeader(std::list<baseComponent*> components);	// DEPRICATED

		// Serialize all components into the packet, only if weve already written the header, return true if wrote, false otherwise.
		void writeComponents(std::list<baseComponent*> components); // DEPRICATED

		// Given an entitiy id and component id, serialize that component into the packet
		void SerializeComponent(GameData::GameData& data, entt::entity id, ComponentID::ComponentID compid);

		template <typename Type>
		void Serialize(Type& component) {
			pk->pack(component);
		}
	};


	class ReflectionSystem : public PrimarySystem::PrimarySystem {
	public:
		// Given the gamedata, flush the update map and broadcast game updates to all users
		void update(GameData::GameData& data);

		void init(GameData::GameData& data) { data.compUpdateMap = std::map<networkID, UpdatePacketHeader::UpdatePacketHeader>(); }

		void DebugDeserialize(GameData::GameData& data, msgpack::sbuffer& stream) { MakeGameUpdate(data, stream); };
		void MakeGameUpdate(GameData::GameData& data, std::string& stream);

	private:

		// Given a stream representing a serialized game update message, write all of the game updates to the game registry
		void MakeGameUpdate(GameData::GameData& data, msgpack::sbuffer& stream);
		void MakeGameUpdateHelper(GameData::GameData& data, const msgpack::object& obj, ComponentID::ComponentID id, entt::entity& enttid);

		template <typename Type>
		void writeComponent(GameData::GameData& data, const msgpack::object& obj, entt::entity& enttid)
		{
			// Check if component exists already in the registry, if yes, fill it with the values in the msgpack object
			if (!data.m_reg.has<Type>(enttid)) {
				auto &enttobj = data.m_reg.emplace<Type>(enttid);
				obj.convert(enttobj);
				//std::cout << obj << std::endl;
			//	enttobj.print();
			}
			else {
				auto &enttobj = data.m_reg.get<Type>(enttid);
				obj.convert(enttobj);
				//std::cout << obj << std::endl;
			//	enttobj.print();
			}
		}
	};
		

}



/*
Steps:
	// At the game tick, the system expects that the game update map is filled
	//1) Reflection system will allocate an update packet, loop through all of the components to be updated and fill the update header with the ids
	//2) reflection system will call serialize on the update packet header
	//3) reflection system will loop through the components in the list, and call serialize onto those to the buffer

	User creates a packet


*/