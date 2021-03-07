#include <MessageIdentifiers.h>
#include "NetworkSystem.h"
#include "Packet.h"
#include "IDTranslationSystem.h"
#include "BitStream.h"
#include "CreateEntity.h"
#include "MessagingSystem.h"

namespace NetworkSystem {
	void NetworkHandler::updateServer(SceneComponent::SceneComponent& data, TranslationSystem::IDTranslation& transSystem)
	{
		RakNet::Packet* pack;

		// Loop through the network instantiation's packet queue, and get some packet of data
		// do whatever you need to do depending on what was passed to us
		for (pack = data.rpi->Receive(); pack; data.rpi->DeallocatePacket(pack), pack = data.rpi->Receive()) {

			switch (pack->data[0])
			{
			case ID_NEW_INCOMING_CONNECTION:
				handleConnection(data, pack);
				break;

			case ID_DISCONNECTION_NOTIFICATION:
				handleDisconnect(data, transSystem, pack);
				break;

			case ID_CONNECTION_LOST:
				handleLostConnection(data, transSystem, pack);
				break;

			case ID_REMOTE_NEW_INCOMING_CONNECTION:
				printf("Remote client has connected.\n");
				handleConnection(data, pack);
				break;

			case ID_REMOTE_DISCONNECTION_NOTIFICATION:
				printf("Remote client has disconnected.\n");
				handleDisconnect(data, transSystem, pack);
				break;

			case ID_REMOTE_CONNECTION_LOST:
				printf("Remote client has lost the connection.\n");
				handleLostConnection(data, transSystem, pack);
				break;

			case ID_NO_FREE_INCOMING_CONNECTIONS:
				printf("The server is full.\n");
				break;

			case ADD_ENTITY:
				printf("Received add entity packet from client.\n");
				addEntity(data, transSystem, pack, data.isServer, 0);
				break;

			case REMOVE_ENTITY:
				break;

			case UPDATE_ENTITY:
				break;

			case CONTROL:
				break;

			default:
				// Some unknown packet type, go to the next packet
				printf("Unknown packet");
				break;
			}
		}
	}

	void NetworkHandler::updateClient(SceneComponent::SceneComponent& data, TranslationSystem::IDTranslation& transSystem)
	{
		// Pointer to some network packet
		RakNet::Packet* pack;

		// Loop through all of the packets in the incoming packet queue
		for (pack = data.rpi->Receive(); pack; data.rpi->DeallocatePacket(pack), pack = data.rpi->Receive()) {
			switch (pack->data[0])
			{

			case ID_CONNECTION_REQUEST_ACCEPTED:
				data.message = "Connection established.\n";
				handleConnection(data, pack);
				break;

			case ID_NO_FREE_INCOMING_CONNECTIONS:
				data.message = "The server is full.\n";
				break;

			case ID_DISCONNECTION_NOTIFICATION:
				data.message = "We have been disconnected.\n";
				break;

			case ID_CONNECTION_LOST:
				data.message = "Connection lost.\n";
				break;

			case ADD_ENTITY:
				printf("Received add entity packet from server.\n");
				addEntity(data, transSystem, pack, data.isServer, 0);
				break;
			case REMOVE_ENTITY:
				break;
			case UPDATE_ENTITY:
				break;
			case CONTROL:
				break;

			default:
				// Some unknown packet type, go to the next packet
				printf("some packet from server.\n");
				break;
			}
		}
	}

	bool NetworkHandler::clientConnect(RakNet::RakPeerInterface* peer, unsigned short port, const char* hostAddress)
	{
		if (!(peer->Connect(hostAddress, port, 0, 0) == RakNet::CONNECTION_ATTEMPT_STARTED)) {
			std::cerr << "Connection attempt failed" << std::endl;
			return false;
		}
		return true;
	}

	void NetworkSystem::NetworkHandler::clientDisconnect(RakNet::RakPeerInterface* peer, const char* hostAddress)
	{
		peer->CloseConnection(RakNet::SystemAddress(hostAddress) , 1);
		std::cout << "Closing connection to " << RakNet::SystemAddress(hostAddress).ToString() << std::endl;
		//RakNet::AddressOrGUID(RakNet::SystemAddress(hostAddress));
	}

	// The server recognizes the connection from a client and creates an empty netID  map for that client
	void NetworkHandler::handleConnection(SceneComponent::SceneComponent& data, RakNet::Packet* pack)
	{
		// If is a server, the rakAddress is not initialized
		if (data.isServer) {
			printf("A client has logged in. Client address: %s \n", pack->systemAddress.ToString());
			data.clientAddressToEntities[pack->systemAddress] = std::list<networkID>();
		}
		// If is a client, the map is not initialized.
		// the rakAddress holds the address of the server
		else {
			printf("Connected to the server. Server address: %s \n", pack->systemAddress.ToString());
			data.rakAddress = pack->systemAddress; // This step is redundant, as the address is hardcoded in the initialization rn
		}
		
	}

	// The server recognizes the disconnection from a client and clears all entities related to that client
	void NetworkHandler::handleDisconnect(SceneComponent::SceneComponent& data, TranslationSystem::IDTranslation& transSystem, RakNet::Packet* pack)
	{
		printf("A client has disconnected. Address: %s \n", pack->systemAddress.ToString());
		for (auto const& netId : data.clientAddressToEntities[pack->systemAddress]) {
			data.m_reg.destroy(transSystem.getEntity(data, netId));
			transSystem.freeID(data, netId); // Free the space for that entity
		}

	}

	void NetworkHandler::handleLostConnection(SceneComponent::SceneComponent& data, TranslationSystem::IDTranslation& transSystem, RakNet::Packet* pack) {
		// Currently this had the same behavior as the way we handle disconnection. TBD
		printf("A client lost the connection. Address: %s \n", pack->systemAddress.ToString());
		printf("This client has %d entities.\n", data.clientAddressToEntities[pack->systemAddress].size());
		for (auto const& netId : data.clientAddressToEntities[pack->systemAddress]) {
			std::cout << "removing entity " << netId << std::endl;
			data.m_reg.destroy(transSystem.getEntity(data, netId));
			transSystem.freeID(data, netId); // Free the space for that entity
		}
	}

	// TODO:
	void NetworkHandler::addEntity(SceneComponent::SceneComponent& data, TranslationSystem::IDTranslation& transSystem, RakNet::Packet* pack, bool isServer, bool responding)
	{
		// If is Server: (we dont need to examine the netid/timestamp of the incomming packet. So make a new message object and sendit)
		if (isServer) {

			// add the entity with entity id given to the m_reg
			auto newEntity = RegWrapper::createEntity(data.m_reg, true);			

			// Allocate a new netId for this entity
			networkID netid = transSystem.createMapping(data, newEntity);
			std::cout << "Registered new entity " << netid << std::endl;
			
			// Append into the client entity map
			data.clientAddressToEntities[pack->systemAddress].push_back(netid);

			// create a new add entity packet
			RakNet::BitStream stream = RakNet::BitStream();
			MessagingSystem::writeAddEntity(stream, netid);


			std::cout << "Registered entity from client " << pack->systemAddress.GetPort() << ", broadcasting..." << std::endl;
			// broadcast to all clients to add a new entity with entity ID, and all components
			data.rpi->Send(&stream, 
				HIGH_PRIORITY, 
				RELIABLE_ORDERED, 
				0, 
				pack->systemAddress, 
				true);
			std::cout << "sent message" << std::endl;

			// TODO:			(also figure out how we want to packetize all of the components)
			// add all components of the entity with the given values to the new entity
		}
		else {
			// If is client:
			// if were not requesting, but responding to a request
			if (responding) {
				std::cout << "Adding entity in client after receiving a packet" << std::endl;
				auto newEntity = RegWrapper::createEntity(data.m_reg, true);

				std::string str = std::string((char*) (pack->data + 1));							// TODO: this may cause a bug, skipping over the first character
				ProtoMessaging::AddRemoveEntityMessage* msg = MessagingSystem::readAddEntity(str);

				// when unpacking input packet, get the netID passed to us
				// Create a new mapping of netID and entity
				transSystem.setMapping(data, msg->netid(), newEntity);
			}
			else {
				// if were requesting:
				std::cout << "Sending out add entity request packet to server" << std::endl;
				RakNet::BitStream stream = RakNet::BitStream();

				MessagingSystem::writeAddEntity(stream, 0);


				// Request an addition of a new entity
				data.rpi->Send(&stream,
					HIGH_PRIORITY,
					RELIABLE_ORDERED,
					0,
					data.rakAddress,
					false);
			}
		}
	}

	// TODO:
	void NetworkHandler::removeEntity(SceneComponent::SceneComponent& data, TranslationSystem::IDTranslation& transSystem, RakNet::SystemAddress& systemAddress, networkID netId, bool isServer)
	{
		// If is Server:
		if (isServer) {
			// Remove the given entity from the m_reg
			transSystem.freeID(data, netId);

			// Create new remove entity packet
			Packets::removeEntityPacket remPack(netId);

			// bitstream to send to
			//RakNet::BitStream bsOut;


			// write the packet to the bitstream (figure out how we want to organize packets)
			// TODO: fill the bitstream, serialize



			// broadcast to all users to remove the given entity using the made packet
			// all users except the owner of this entity
		//	data.rpi->Send(&bsOut, HIGH_PRIORITY, RELIABILITY, 0, (Incomming packet address), true);

		}
		else {
			// If is client:
			// Remove the given entity from the m_reg
			data.m_reg.destroy(transSystem.getEntity(data, netId));

			// Send to the server that we are removing this entity

			// Create new remove identity packet
			Packets::removeEntityPacket remPack(netId);

			// bitstream to send to
		//	RakNet::BitStream bsOut;

			// write the packet to the bitstream

			// send the packet to the server
		//	data.rpi->Send(&bsOut, HIGH_PRIORITY, RELIABILITY, 0, (Server address), true);
		}
	}

}

