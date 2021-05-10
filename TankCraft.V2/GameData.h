#pragma once
#include <entt/entt.hpp>
#include <string>
#include <RakPeerInterface.h>
#include "IDTranslationComponent.h"
#include "Tanks.pb.h"
#include "ComponentID.h"
#include "UpdatePacketHeader.h"
#include <msgpack.hpp>

namespace GameData { class GameData; };

// TODO: unlock doesnt actually do anything right now..., so implement it
// The base component, its here now because it bugged out when it was in its own header
typedef struct baseComponent_ {
	ComponentID::ComponentID CompId;
	bool networked; // Temporarily underlined so i can see when it is used

	baseComponent_() { CompId = ComponentID::Base; networked = false; }
	virtual ~baseComponent_() = default;
	virtual void write(ProtoMessaging::UpdateEntityMessage& message, networkID netid) { std::cout << "base write " << std::endl; }
	virtual void lock() { };
	void unlock(GameData::GameData& data, const entt::entity& entity) ; // { std::cout << " base unlock" << std::endl; }
	bool isNetworked() { return networked; }

	virtual void Serialize(msgpack::sbuffer& sbuf) {};
	virtual void print() {  }

	virtual int size() { return sizeof(ComponentID::ComponentID) + sizeof(bool); }
	//virtual void read(ProtoMessaging::UpdateEntityMessage& message, networkID netid, int index) {}
} baseComponent;

namespace PrimarySystem {
	class PrimarySystem {
	public:
		virtual void init(GameData::GameData& data) {
			std::cout << "primary system initiated" << std::endl;
		};

		virtual void update(GameData::GameData& data) {};

		PrimarySystem() = default;

		~PrimarySystem() = default;
	};
}

/*
*  Class containing all of the global data to the system.
*/
namespace GameData {
	class GameData {
	public:
		// The main UI
		// This data is the SAME across all clients
		std::vector<std::vector<char>> map;

		// main registry
		// This data is DIFFERENT across the server and all clients
		entt::registry m_reg;

		// Peer to Peer interface with raknet
		RakNet::RakPeerInterface* rpi;

		// The translation of net to entity structure
		// This data should be SERVER ONLY and ONE PER CLIENT
		std::map<networkID, entt::entity> netToEnttid;	
		std::map<entt::entity, networkID> enttToNetidid;

		// The map from each client to its list of entities
		std::map<RakNet::SystemAddress, std::list<networkID>> clientAddressToEntities;

		// The map of updates to broadcast to all nodes
		std::map<networkID, UpdatePacketHeader::UpdatePacketHeader> compUpdateMap;

		// The log of most rescent updates (to be sent to any user loging in)
		std::map<networkID, UpdatePacketHeader::UpdatePacketHeader> compLog;

		// the address were connected to
		RakNet::SystemAddress rakAddress;

		std::list<PrimarySystem::PrimarySystem*> primarySystemList;

		// Whether this scene is the server or a client 
		bool isServer;

		int maxClients;

		// Whether the first client has logged in or not
		bool first;

		std::string *userName;
	};
}

