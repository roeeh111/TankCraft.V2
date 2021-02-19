#include "NetworkSystem.h"
#include <MessageIdentifiers.h>

namespace NetworkSystem {

	void updateClient(RakNet::RakPeerInterface* peer)
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


				// Call makeClientUpdate with a new thread


			//	printf("Message with identifier %i has arrived.\n", pack->data[0]);
				break;
			}
		}


		// Iterate through userInputQueue



	}






	void makeClientUpdate(Packet pack)
	{
	}
}

