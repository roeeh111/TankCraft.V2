#pragma once
#include <msgpack.hpp>
#include <vector>
#include "GameData.h"
#include "ComponentID.h"
#include "Components.h"
#include "IDTranslationComponent.h"
#include "IDTranslationSystem.h"



namespace ReflectionSystem {



	class UpdatePacketHeader {
	public:
		std::vector<ComponentID::ComponentID> ids; // The array of component ids
		networkID netid;						   // The netid attached to these components
		MSGPACK_DEFINE(netid, ids);

		UpdatePacketHeader(networkID netid_) {
			ids = std::vector<ComponentID::ComponentID>();
			netid = netid_;
		}
		UpdatePacketHeader() {
			ids = std::vector<ComponentID::ComponentID>();
			netid = 0;
		}
		~UpdatePacketHeader() = default;
	};

	class UpdatePacket {
	public:

		UpdatePacket(networkID netid_) {
			header = UpdatePacketHeader(netid_);
			pk = new msgpack::packer<msgpack::sbuffer>(&sbuf);
		}
		~UpdatePacket() { delete pk; };
		
		msgpack::sbuffer& Serialize(std::list<baseComponent*> components);

		// Serialize a component into theclass buffer
	private:
		msgpack::sbuffer sbuf;
		msgpack::packer<msgpack::sbuffer>* pk;
		UpdatePacketHeader header;

		// Write all component id's into the header
		void writeHeader(std::list<baseComponent*> components);

		// Serialize all components into the packet, only if weve already written the header, return true if wrote, false otherwise.
		void writeComponents(std::list<baseComponent*> components);

		template <typename Type>
		void Serialize(Type& component) {
			pk->pack(component);
		}
	};


	// Given a stream representing a serialized game update message, write all of the game updates to the game registry
	void MakeGameUpdate(GameData::GameData& data, std::string& stream);
	void MakeGameUpdate(GameData::GameData& data, msgpack::sbuffer& stream);
	void MakeGameUpdateHelper(GameData::GameData& data, const msgpack::object& obj, ComponentID::ComponentID id, entt::entity& enttid);

	// Given the gamedata, flush the update map and broadcast game updates to all users
	void FlushGameUpdate(GameData::GameData& data);


	template <typename Type>
	void writeComponent(GameData::GameData& data, const msgpack::object& obj, entt::entity &enttid)
	{		 
		// Check if component exists already in the registry, if yes, fill it with the values in the msgpack object
		if (!data.m_reg.has<Type>(enttid)) {
			auto enttobj = data.m_reg.emplace<Type>(enttid);
			obj.convert(enttobj);
		}
		else {
			auto enttobj = data.m_reg.get<Type>(enttid);
			obj.convert(enttobj);
		}
	}
		

}



/*
Steps:
	// At the game tick, the system expects that the game update map is filled
	//1) Reflection system will allocate an update packet, loop through all of the components to be updated and fill the update header with the ids
	//2) reflection system will call serialize on the update packet header
	//3) reflection system will loop through the components in the list, and call serialize onto those to the buffer

	User creates a packet


*/