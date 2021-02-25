#pragma once
#include <entt/entt.hpp>
#include "Components.h"
#include "Serialize.h"
#include "IDTranslationSystem.h"
#include "NetworkFields.h"

namespace Packets {
	
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
		updateEntityPacket(entt::entity entityID_, enum ComponentView::ComponentID compID_) { type = updateEntity; entityID = entityID_; compID = compID; }

		// Serialization functions
		void read();
		void write();
	protected:
		entt::entity entityID;
		enum ComponentView::ComponentID compID;
		// No data field yet, base class of updateEntity
	};

	class removeEntityPacket : public Packet {
	public:
		removeEntityPacket() { type = removeEntity; }
		removeEntityPacket(networkID netID_) { type = removeEntity; netID = netID_; }

		// Serialization functions
		void read();
		void write();
	protected:
		networkID netID;
		// No data field yet, base class of removeEntity
	};

	class addEntityPacket : public Packet {
	public:
		addEntityPacket() { type = addEntity; }
		addEntityPacket(entt::entity entityID_, enum ComponentView::ComponentID compID_) { type = addEntity; entityID = entityID_; compID = compID; }

		// Serialization functions
		void read();
		void write();
	protected:
		entt::entity entityID;
		enum ComponentView::ComponentID compID;
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
		HealthUpdatePacket() { type = updateEntity; ComponentView::health(); }
		HealthUpdatePacket(entt::entity entityID_, ComponentView::health data_) { entityID = entityID_; compID = ComponentView::Health; data = data_; }
	private:
		ComponentView::health data;
	};
}



/**

3 packet types:
	- Control packet (sends some controls from client to server)		// easy 
	- add entitity (sends to add an entitiy with X data) // harder, because of data field X (block of data)
	- remove entity (sends to remove some entity)  // easy, just remove this entitiy
	- game update (sends to change a list of components X)     // hard, need to define how we want to send components
	- login (add new user)	// easy 
	- logout (remove user) // easy 


	add entity, and game update carry actual data to do something with

	Approaches to organize data:
	1) just Hard code a bunch of packets, go update this specific entitity, with these fields
		- all pre organized, both ends know
		faults:
			- not scalable
			- break the advantage of ecs

	2) Build a packet with metatdata every time:
		packet header(entity e, enum component x, enum component y, enum component z) | (x, y, z) // blocks of data
		faults:
			- if you want to update only parts of components, or if component x can vary in size you cant
				(or it adds some metadata)

	3) Build a packet with metadata every time, and add metadata to the fields
	packet header(entity e, enum component x, enum compontent y) | componentx:(enum field 1, enum field 2) (1, 2) | componenty: (enum field 1) (1)
		- most robust, can have dynamic component sizes
		fault:
			- buttload of metadata (can have more or less depending on how you make it)

			


























