#pragma once
#include <entt/entt.hpp>
#include "Components.h"
#include "Serialize.h"

namespace NetworkSystem {
	
	// TODO: Add more potential packet types
	enum Packet_Type {
		updateEntity,
		addEntity,
		removeEntity,
		controlInput
	};

	// Packet base type, all packets have a packet_type
	class Packet {
	public:
		Packet() { type = updateEntity; }
	protected:
		enum Packet_Type type;
	};

	// Base class for a game update packet. all game updates have an entityID and ComponentID.
	// Inheriting classes will have an additional data and read/write fields
	class updateEntityPacket : public Packet {
	public:
		updateEntityPacket() { type = updateEntity; }
		updateEntityPacket(entt::entity entityID_, enum GameView::ComponentID compID_) { type = updateEntity; entityID = entityID_; compID = compID; }

		// Serialization functions
		void read();
		void write();
	protected:
		entt::entity entityID;
		enum GameView::ComponentID compID;
		// No data field yet, base class of updateEntity
	};

	class removeEntityPacket : public Packet {
	public:
		removeEntityPacket() { type = removeEntity; }
		removeEntityPacket(entt::entity entityID_) { type = removeEntity; entityID = entityID_; }

		// Serialization functions
		void read();
		void write();
	protected:
		entt::entity entityID;
		// No data field yet, base class of removeEntity
	};

	class addEntityPacket : public Packet {
	public:
		addEntityPacket() { type = addEntity; }
		addEntityPacket(entt::entity entityID_, enum GameView::ComponentID compID_) { type = addEntity; entityID = entityID_; compID = compID; }

		// Serialization functions
		void read();
		void write();
	protected:
		entt::entity entityID;
		enum GameView::ComponentID compID;
		// No data field yet, base class of addEntity
	};

	class controlInputPacket : public Packet {
	public:
		controlInputPacket() { type = controlInput; }
		//TODO: make templates for input packet

		// Serialization functions
		void read();
		void write();
	protected:
		entt::entity entityID;
		// No data field yet, base class of controlInput
	};

	// Health Component update packet
	class HealthUpdatePacket : public updateEntityPacket {
	public: 
		HealthUpdatePacket() { type = updateEntity; GameView::health(); }
		HealthUpdatePacket(entt::entity entityID_, GameView::health data_) { entityID = entityID_; compID = GameView::Health; data = data_; }
	private:
		GameView::health data;
	};
}
