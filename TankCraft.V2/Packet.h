#pragma once
#include <entt/entt.hpp>
#include <RakPeerInterface.h>
#include <entt/entt.hpp>
#include <msgpack.hpp>
#include <sstream>
#include <iostream>
#include "Components.h"
#include "IDTranslationComponent.h"
#include <MessageIdentifiers.h>

#define UPDATE_LIMIT 10
#define BUFSIZE 5000


/*
* header with all our packets, still needs a lot of work. 
*
*/

/*


namespace Packets {
	

	// Packet base type, all packets have a packet_type
	struct Packet {
	public:
		Packet() { type = UPDATE_ENTITY; }
	protected:
		enum DefaultMessageIDTypes type;
	};

	// Packet saying to remove an entity with the given netid
	struct removeEntityPacket : public Packet {
	public:
		removeEntityPacket(networkID netID_) { type = REMOVE_ENTITY; netID = netID_; }

	protected:
		networkID netID;
		// No data field yet, base class of removeEntity
	};

	// Packet saying to add an entity with the given netid
	struct addEntityPacket {
		RakNet::MessageID type;
		networkID netID;
		addEntityPacket(networkID netID_) { type = ADD_ENTITY; netID = netID_; }
	};


	// Packet saying that user with entity netid inputed control controls
	struct controlPacket : public Packet {
	public:
		controlPacket(networkID netID_, ComponentView::userInput& controls_) { type = CONTROL; controls = controls_; }

		// Serialization functions
		void read();
		void write();
	protected:
		networkID netID;
		ComponentView::userInput controls;
	};



	// Base class for a game update packet. all game updates have an entityID and ComponentID.
	// Inheriting classes will have an additional data and read/write fields
	struct updateEntityPacket : public Packet {
	public:
		updateEntityPacket() { type = UPDATE_ENTITY; }
		updateEntityPacket(networkID netID_, ComponentView::ComponentID compID_) { type = UPDATE_ENTITY; netID = netID_; compID = compID; }

		// Serialization functions
		void read();
		void write();
	protected:
		networkID netID;
		ComponentView::ComponentID compID;
		// No data field yet, base class of updateEntity
	};


	class UpdatePacket {
	private:
		enum DefaultMessageIDTypes type;
		networkID netID;
		ComponentView::ComponentID compID;
		std::stringstream inout;

	public:
		UpdatePacket() {
			type = UPDATE_ENTITY;
			inout = std::stringstream();
		}

		UpdatePacket(networkID netID_) {
			type = UPDATE_ENTITY;
			netID = netID_;
			inout = std::stringstream();
		//	inout << type;
		//	inout << netID;
		}


		// NEED SOME RUNTIME TRANSLATION SYSTEM FROM COMPONENT TYPE TO AN ACTUAL COMPONENT

		// want to get a class type from an enum

		void write() {
			//msgpack::pack(inout, );
		}

		void read(std::stringstream &buffer) {
			
		//	memcpy(&type, str.data(), sizeof(Packet_Type));
		//	memcpy(&netID, str.data() + sizeof(Packet_Type), sizeof (networkID));
			std::string str(buffer.str());


			msgpack::object_handle oh =
				msgpack::unpack(str.data(), str.size());

//				msgpack::unpack(str.data() + sizeof(Packet_Type) + sizeof(networkID), str.size());

			// deserialized object is valid during the msgpack::object_handle instance is alive.
			msgpack::object deserialized = oh.get();

			std::cout << type << std::endl;
			std::cout << netID << std::endl;

			// msgpack::object supports ostream.
			std::cout << deserialized << std::endl;

			// convert msgpack::object instance into the original type.
			// if the type is mismatched, it throws msgpack::type_error exception.
		//	msgpack::type::tuple<int, bool, std::string> dst;
		//	deserialized.convert(dst);
		}

	};



}

*/

/*
* Another method with msgpack:
* 
* put a wrapper around msgpack.
* 
* packet structure:
* entity id
* component id
* stringstream to write to
* 
* 
* writeToPacket(component) {
* write entity id to stringstream
* do the msgpack write to the stringstream
* }
* 
* readFromPacket(std::string) {
*  get component type from 1'st element
*  call unpack to deserialize
*  return a pair<enum component type, string data>
*}
* 
* Requires some translation system for how to get from component type to component cast, but that is needed anyway
* 
* 
*/




/**

3 packet types:
	- Control packet (sends some controls from client to server)		// easy 
	- remove entity (sends to remove some entity)  // easy, just remove this entitiy
	- login (add new user)	// easy 
	- logout (remove user) // easy 
	- update entity (sends to change a list of components X)     // hard, need to define how we want to send components
	- add entitity (sends to add an entitiy with X data) // harder, because of data field X (block of data)


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

**/






















