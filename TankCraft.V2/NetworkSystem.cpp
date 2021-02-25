#include <MessageIdentifiers.h>
#include "NetworkSystem.h"
#include "Packet.h"

namespace NetworkSystem {
	void NetworkHandler::updateServer(SceneData::SceneData& data)
	{
		RakNet::Packet* pack;

		// Loop through the network instantiation's packet queue, and get some packet of data
		// do whatever you need to do depending on what was passed to us
		for (pack = data.rpi->Receive(); pack; data.rpi->DeallocatePacket(pack), pack = data.rpi->Receive()) {
			switch (pack->data[0])
			{
			case ID_REMOTE_DISCONNECTION_NOTIFICATION:
				printf("Another client has disconnected.\n");

				handleDisconnect(true);

				break;
			case ID_REMOTE_CONNECTION_LOST:
				printf("Another client has lost the connection.\n");

				handleDisconnect(true);

				break;
			case ID_REMOTE_NEW_INCOMING_CONNECTION:
				printf("Another client has connected.\n");

				handleConnection();

				break;
			case ID_NEW_INCOMING_CONNECTION:
				printf("A connection is incoming.\n");

				handleConnection();

				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				printf("The server is full.\n");
				break;
			case ID_DISCONNECTION_NOTIFICATION:
				printf("A client has disconnected.\n");

				handleDisconnect(true);

				break;
			case ID_CONNECTION_LOST:
				printf("A client lost the connection.\n");

				handleDisconnect(true);

				break;
			default:
				//	printf("Message with identifier %i has arrived.\n", pack->data[0]);

				// Call makeServerUpdate with a new thread (NOTE DONT DEALLOCATE THE PACKET HERE!!!!!!!)
				makeServerUpdate(data.m_reg, pack);

				break;
			}
		}


	}




	void NetworkHandler::updateClient(SceneData::SceneData& data)
	{
		// Pointer to some network packet
		RakNet::Packet* pack;

		// Loop through all of the packets in the incoming packet queue
		for (pack = data.rpi->Receive(); pack; data.rpi->DeallocatePacket(pack), pack = data.rpi->Receive()) {
			switch (pack->data[0])
			{

			case ID_CONNECTION_REQUEST_ACCEPTED:
				printf("Our connection request has been accepted.\n");
				break;

			case ID_NO_FREE_INCOMING_CONNECTIONS:
				printf("The server is full.\n");
				return;

			case ID_DISCONNECTION_NOTIFICATION:
				printf("We have been disconnected.\n");
				return;

			case ID_CONNECTION_LOST:
				printf("Connection lost.\n");
				return;

			default: 				// Message with identifier pack->data[0]
													// TODO: add identifiers to the list of possibilities, And check for those
													// (identifiers being packet type)


				// Call makeClientUpdate with a new thread (NOTE DONT DEALLOCATE THE PACKET HERE!!!!!!!)
				makeClientUpdate(data.m_reg, pack);
				break;

			//	printf("Message with identifier %i has arrived.\n", pack->data[0]);
			}
		}

		// TODO:
		// Handle User Input:

		// Assume that were tick based, so get the user input component (most rescent control to the tick),
		// if that component is dirty, build a new control packet and send it to the server
		// else do nothing

	}

	bool NetworkHandler::clientConnect(RakNet::RakPeerInterface* peer, unsigned short port, const char* hostAddress)
	{
		std::cout << "Starting up a client" << std::endl;

		if (!(peer->Connect(hostAddress, port, 0, 0) == RakNet::CONNECTION_ATTEMPT_STARTED)) {
			std::cerr << "Connection attempt failed" << std::endl;
			return false;
		}
		return true;
	}


	// TODO:
	void NetworkHandler::makeClientUpdate(entt::registry &m_reg, RakNet::Packet* pack)
	{
		// Deserialize packet
		// get the entity
		// find the component that entity is refrencing
		// Aquire lock for component
		// change the fields of component
		// release lock for component
	}

	// TODO:
	void NetworkHandler::makeServerUpdate(entt::registry &m_reg, RakNet::Packet* pack)
	{
		// Validate packet??
		// Deserealize the packet
		// Execute what the controls would do to the server (probably a function from another system)
		// Append all changes to the changedComponentQueue
	}


	// TODO:	(do we need to keep track of the list of current connections???)
	void NetworkHandler::handleConnection()
	{
		// Broadcast to all users that a new user has spawned in, and where ?????
		// Send the current game state to the user
		//		meaning: Send all entities, and all components to the user so they may add it
	}

	// TODO:
	void NetworkHandler::addEntity(SceneData::SceneData& data, TranslationSystem::IDTranslation& transSystem, bool isServer)
	{
		// If is Server:
		if (isServer) {
			// add the entity with entity id given to the m_reg
			auto newEntity = data.m_reg.create();

			// Allocate a new netId for this entity
			networkID netind = transSystem.createMapping(data, newEntity);


			// TODO:			(also figure out how we want to packetize all of the components)
			// add all components of the entity with the given values to the new entity
			// broadcast to all clients to add a new entity with entity ID, and all components
		}
		else {
			// If is client:
				// if client doesnt have an entity with this netid 
				// add the entity with the entity id given to the m_reg
				auto newEntity = data.m_reg.create();


				// TODO: when unpacking input packet, get the netID passed to us
				// Create a new mapping of netID and entity

				// TODO: 
				// add all components of the entitty with the given valies to the new entity
		}
	}

	
	// TODO:
	void NetworkHandler::removeEntity(SceneData::SceneData& data, TranslationSystem::IDTranslation& system, networkID netId, bool isServer)
	{
		// If is Server:
		if (isServer) {
			// Remove the given entity from the m_reg
			system.freeID(data, netId);

			// Create new remove entity packet
			Packets::removeEntityPacket remPack(netId);

			// bitstream to send to
			RakNet::BitStream bsOut;


			// write the packet to the bitstream (figure out how we want to organize packets)
			// TODO: fill the bitstream, serialize



			// broadcast to all users to remove the given entity using the made packet
			// all users except the owner of this entity
		//	data.rpi->Send(&bsOut, HIGH_PRIORITY, RELIABILITY, 0, (Incomming packet address), true);

		}
		else {
			// If is client:
			// Remove the given entity from the m_reg
			data.m_reg.destroy(system.getEntity(data, netId));

			// Send to the server that we are removing this entity

			// Create new remove identity packet
			Packets::removeEntityPacket remPack(netId);

			// bitstream to send to
			RakNet::BitStream bsOut;

			// write the packet to the bitstream

			// send the packet to the server
		//	data.rpi->Send(&bsOut, HIGH_PRIORITY, RELIABILITY, 0, (Server address), true);
		}
	}
	
	// TODO:
	void NetworkHandler::handleDisconnect(bool isServer)
	{
		// If is Server:
		// TODO: how do we keep track of whose client is what entity?
		// Find all entities that are tagged with the given client's id
		// call removeEntity on all those entities
		// TODO: do we run some sort of raknet disconnect function?

		// If is client:
		// TODO: define if we should do anything as a client

	}

}

