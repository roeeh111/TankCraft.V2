#include "NetworkSystem.h"
#include <MessageIdentifiers.h>

namespace NetworkSystem {
	void updateServer(entt::registry* m_reg, RakNet::RakPeerInterface* peer)
	{
		RakNet::Packet* pack;

		// Loop through the network instantiation's packet queue, and get some packet of data
// check the data value and 
		for (pack = peer->Receive(); pack; peer->DeallocatePacket(pack), pack = peer->Receive()) {
			switch (pack->data[0])
			{
			case ID_REMOTE_DISCONNECTION_NOTIFICATION:
				printf("Another client has disconnected.\n");

				// TODO: implement disconnect function
				handleDisconnect();

				break;
			case ID_REMOTE_CONNECTION_LOST:
				printf("Another client has lost the connection.\n");

				// TODO: implement disconnect function
				handleDisconnect();

				break;
			case ID_REMOTE_NEW_INCOMING_CONNECTION:
				printf("Another client has connected.\n");

				// TODO: implement connect function
				handleConnection();

				break;
			case ID_NEW_INCOMING_CONNECTION:
				printf("A connection is incoming.\n");

				// TODO: implement connect function
				handleConnection();

				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				printf("The server is full.\n");
				break;
			case ID_DISCONNECTION_NOTIFICATION:
				printf("A client has disconnected.\n");

				// TODO: implement disconnect function
				handleDisconnect();

				break;
			case ID_CONNECTION_LOST:
				printf("A client lost the connection.\n");

				// TODO: implement disconnect function
				handleDisconnect();

				break;
			default:
			//	printf("Message with identifier %i has arrived.\n", pack->data[0]);

				// Call makeServerUpdate with a new thread (NOTE DONT DEALLOCATE THE PACKET HERE!!!!!!!)


				break;
			}
		}


	}




	void updateClient(entt::registry* m_reg, RakNet::RakPeerInterface* peer)
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
													// TODO: add exit function
				break;
			case ID_DISCONNECTION_NOTIFICATION:
				printf("We have been disconnected.\n");
													// TODO: add exit function
				break;
			case ID_CONNECTION_LOST:
				printf("Connection lost.\n");
													// TODO: add exit function
				break;
			default: 				// Message with identifier pack->data[0]
													// TODO: add identifiers to the list of possibilities, And check for those
													// (identifiers being packet type)


				// Call makeClientUpdate with a new thread (NOTE DONT DEALLOCATE THE PACKET HERE!!!!!!!)


			//	printf("Message with identifier %i has arrived.\n", pack->data[0]);
		//		break;
			}
		}

		// TODO:
		// Iterate through userInputQueue
			// build new control packet from control input
			// send control packet up to server
			// Delete local copy of packet
	}


	// TODO:
	void makeClientUpdate(entt::registry* m_reg, RakNet::Packet* pack)
	{
		// Deserialize packet
		// get the entity
		// find the component that entity is refrencing
		// Aquire lock for component
		// change the fields of component
		// release lock for component
	}

	// TODO:
	void makeServerUpdate(entt::registry* m_reg, RakNet::Packet* pack)
	{
		// Validate packet??
		// Execute what the controls would do to the server (probably a function from another system)
	}
}

