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
		
		// Serialize the update packet into a buffer
		msgpack::sbuffer& Serialize(GameData::GameData &data);
		// Serialize a component into theclass buffer
	private:
		msgpack::sbuffer sbuf;
		msgpack::packer<msgpack::sbuffer>* pk;
		UpdatePacketHeader::UpdatePacketHeader header;

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

		void init(GameData::GameData& data) {
			data.compUpdateMap = std::map<networkID, UpdatePacketHeader::UpdatePacketHeader>(); 
			data.compLog = std::map<networkID, UpdatePacketHeader::UpdatePacketHeader>();
		}

		//void DebugDeserialize(GameData::GameData& data, msgpack::sbuffer& stream) { MakeGameUpdate(data, stream); };
		void MakeGameUpdate(GameData::GameData& data, std::string& stream);

	private:

		// Given a stream representing a serialized game update message, write all of the game updates to the game registry
		void MakeGameUpdateHelper(GameData::GameData& data, const msgpack::object& obj, ComponentID::ComponentID id, entt::entity& enttid);

		// Helper function to handle the removal of components given the header of a remove component packet
		void RemoveComponents(GameData::GameData& data, UpdatePacketHeader::UpdatePacketHeader& header);
		void RemoveComponentsHelper(GameData::GameData& data, entt::entity id, ComponentID::ComponentID compid);
		template <typename Type>
		void writeComponent(GameData::GameData& data, const msgpack::object& obj, entt::entity& enttid)
		{
			// Check if component exists already in the registry, if yes, fill it with the values in the msgpack object
			if (!data.m_reg.has<Type>(enttid)) {
				auto &enttobj = data.m_reg.emplace<Type>(enttid);
				obj.convert(enttobj);
				//std::cout << obj << std::endl;
		//		std::cout << "entity "  << (int) enttid << " does not have component, had to emplace..." << std::endl;
				enttobj.print();
			}
			else {
				auto &enttobj = data.m_reg.get<Type>(enttid);
				obj.convert(enttobj);
				//std::cout << obj << std::endl;
				enttobj.print();
			}
		}
	};

	// Helper function to log a component change within the update map
	void UpdateLog(GameData::GameData& data, UpdatePacketHeader::UpdatePacketHeader header);

	// Helper function to remove a component from the log
	void RemoveLogComponent(GameData::GameData& data, ComponentID::ComponentID compID);
		

}



/*
Steps:
	// At the game tick, the system expects that the game update map is filled
	//1) Reflection system will allocate an update packet, loop through all of the components to be updated and fill the update header with the ids
	//2) reflection system will call serialize on the update packet header
	//3) reflection system will loop through the components in the list, and call serialize onto those to the buffer

	User creates a packet


*/

/*
* If a dev wants to add a new component:
* 1) enherit from base component
* 2) fill out the serialize function
* 3) write the convert and pack methods
* 4) add a case to the make update helper function
* 5) add a case to the serialize component function
* 6) add a case to the componentID->remove helper function
*/