#pragma once
#include "TankObjects.h"
#include <entt/entt.hpp>

namespace NetworkSystem {
	

	/*
		TODO: add the rest of the packet types
	*/


	enum Packet_Type {
		gameUpdate,
		addEntity,
		removeEntity,
		controlInput
	};


	// Packet base type, all packets have a packet_type
	class Packet {
	public:
		Packet() { type = gameUpdate; }
	protected:
		enum Packet_Type type;

	};

	// Base class for a game update packet. all game updates have an entityID and ComponentID.
	// Inheriting classes will have an additional data and read/write fields
	class GameUpdatePacket : public Packet {
	public:
		GameUpdatePacket() { type = gameUpdate; }
		GameUpdatePacket(entt::entity entityID_, enum Tanks::ComponentID compID_) { type = gameUpdate, entityID = entityID_, compID = compID; }

		// Serialization functions
		void read();
		void write();
	protected:
		entt::entity entityID;
		enum Tanks::ComponentID compID;
		// No data field yet, base class of gameupdatepacket
	};


	// Health Component update packet
	class HealthUpdatePacket : public GameUpdatePacket {
	public: 
		HealthUpdatePacket() { type = gameUpdate; Tanks::health(); }
		HealthUpdatePacket(entt::entity entityID_, Tanks::health data_) { entityID = entityID_, compID = Tanks::Health, data = data_; }
	private:
		Tanks::health data;
	};





}
