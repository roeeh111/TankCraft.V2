#include "NetworkSystem.h"
#include <MessageIdentifiers.h>

namespace NetworkSystem {
	void NetworkHandler::updateServer(entt::registry &m_reg, RakNet::RakPeerInterface* peer)
	{
		RakNet::Packet* pack;

		// Loop through the network instantiation's packet queue, and get some packet of data
		// do whatever you need to do depending on what was passed to us
		for (pack = peer->Receive(); pack; peer->DeallocatePacket(pack), pack = peer->Receive()) {
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
				makeServerUpdate(m_reg, pack);

				break;
			}
		}


	}




	void NetworkHandler::updateClient(entt::registry &m_reg, RakNet::RakPeerInterface* peer)
	{
		// Pointer to some network packet
		RakNet::Packet* pack;

		// Loop through all of the packets in the incoming packet queue
		for (pack = peer->Receive(); pack; peer->DeallocatePacket(pack), pack = peer->Receive()) {
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
				makeClientUpdate(m_reg, pack);
				break;

			//	printf("Message with identifier %i has arrived.\n", pack->data[0]);
			}
		}

		// TODO:
		// Iterate through userInputQueue
			// build new control packet from control input
			// send control packet up to server
			// Delete local copy of packet
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


	// TODO:
	void NetworkHandler::handleConnection()
	{
		// Add this connection to the list of connections?? maybe raknet already has it
		// Broadcast to all users that a new user has spawned in, and where
		// Call addEntity
		// Send the current game state to the user
		//		meaning: Send all entities, and all components to the user so they may add it
	}

	// TODO:
	void NetworkHandler::addEntity(entt::registry& m_reg, bool isServer)
	{
		// If is Server:
		// add the entity with entity id given to the m_reg
		// add all components of the entity with the given values to the new entity
		// broadcast to all clients to add a new entity with entity ID, and all components

		// If is client:
		// add the entity with the entity id given to the m_reg
		// add all components of the entitty with the given valies to the new entity

	}

	// TODO:
	void NetworkHandler::removeEntity(bool isServer)
	{
		// If is Server:
		// Remove the given entity from the m_reg
		// broadcast to all users to remove the given entity

		// If is client:
		// Remove the given entity from the m_reg

		// Note: i think register.remove/delete will remove the entity and destroy all its data for us
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

