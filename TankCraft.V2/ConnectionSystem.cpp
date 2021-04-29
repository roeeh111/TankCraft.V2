#include <MessageIdentifiers.h>
#include "ConnectionSystem.h"
#include "IDTranslationSystem.h"
#include "BitStream.h"
#include "RegWrappers.h"
#include "MessagingSystem.h"
#include "MovementSystem.h"
#include "NetworkUtilitySystem.h"
#include "ReflectionSystem.h"

namespace ConnectionSystem{

	void ConnectionSystem::init(GameData::GameData& data) {
		// Instantiate the network instance for our peer interface
		std::cout << "initializing data.rpi" << std::endl;
		data.rpi = RakNet::RakPeerInterface::GetInstance();
		if (data.isServer) {
			// Local socket to use for communication
			RakNet::SocketDescriptor sd(SERVER_PORT, 0);

			// Startup the peer instance with our binded socket
			if (!(data.rpi->Startup(data.maxClients, &sd, 1) == RakNet::RAKNET_STARTED)) {
				std::cerr << "Failed to startup on socket!" << std::endl;
			}
			data.rpi->SetMaximumIncomingConnections(data.maxClients);
			printf("server started\n");
		}
		else {
			RakNet::SocketDescriptor sd;
			if (!(data.rpi->Startup(1, &sd, 1) == RakNet::RAKNET_STARTED)) {
				std::cerr << "Failed to startup on socket!" << std::endl;
			}
			else {
				std::cout << "Client started" << std::endl;
				clientConnect(data.rpi, SERVER_PORT, "127.0.0.1");
				data.rakAddress = RakNet::SystemAddress("127.0.0.1|" + SERVER_PORT); // save the server address
			}
		}
	}

	void ConnectionSystem::update(GameData::GameData& data)
	{
		if (data.isServer) {
			updateServer(data);
		}
		else {
			updateClient(data);
		}
	}

	void ConnectionSystem::updateServer(GameData::GameData& data)
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

			case ID_CONNECTION_LOST:
				handleLostConnection(data, pack);
				break;

				// In an ideal server authoritative design, Clients shouldn't send this packet.
				// This is for debug purposes
			case ADD_ENTITY:
				printf("Received add entity packet from client.\n");
				NetworkUtilitySystem::addEntity(data, pack, data.isServer, true);
				break;

				// In an ideal server authoritative design, Clients shouldn't send this packet
				// This is for debug purposes
			case REMOVE_ENTITY:
				printf("Received remove entity packet from client.\n");
				NetworkUtilitySystem::removeEntity(data, pack, 0, true, false);
				break;

				// In an ideal server authoritative design, Clients shouldn't send this packet
				// This is for debug purposes
			case UPDATE_ENTITY:
				break;

			case CONTROL:
				printf("Received control packet from client, inputting controls.\n");
				NetworkUtilitySystem::handleControl(data, pack);
				break;

			case LOGIN:
			{
				printf("Received log in packet.\n");
				handleLogin(data, pack);
				break;
			}
			default:
				// Some unknown packet type, ignore
				printf("Unknown packet, id: " + pack->data[0]);
				break;
			}
		}
	}

	void ConnectionSystem::updateClient(GameData::GameData& data)
	{
		// Pointer to some network packet
		RakNet::Packet* pack;

		// Loop through all of the packets in the incoming packet queue
		for (pack = data.rpi->Receive(); pack; data.rpi->DeallocatePacket(pack), pack = data.rpi->Receive()) {
			switch (pack->data[0])
			{
			case ID_CONNECTION_REQUEST_ACCEPTED:
				std::cout << "Connection established." << std::endl;
				handleConnection(data, pack);
				break;

			case ID_CONNECTION_LOST:
				std::cout << "Connection lost." << std::endl;
				break;

			case ADD_ENTITY:
				printf("Received add entity packet from server.\n");
				NetworkUtilitySystem::addEntity(data, pack, false, false);
				break;

			case REMOVE_ENTITY:
				printf("Received remove entity packet from server.\n");
				NetworkUtilitySystem::removeEntity(data, pack, 0, false, true);
				break;

			case UPDATE_ENTITY:
			{
				printf("Received update entity packet from server.\n");
				std::string str = std::string((char*)(pack->data + 1), pack->length - 1);

				ReflectionSystem::ReflectionSystem reflectionSystem;
				reflectionSystem.MakeGameUpdate(data, str);
				break;
			}

			default:
				// Some unknown packet type, go to the next packet
				printf("some packet from server.\n");
				break;
			}
		}
	}

	bool ConnectionSystem::clientConnect(RakNet::RakPeerInterface* peer, unsigned short port, const char* hostAddress)
	{
		if (!(peer->Connect(hostAddress, port, 0, 0) == RakNet::CONNECTION_ATTEMPT_STARTED)) {
			std::cerr << "Connection attempt failed" << std::endl;
			return false;
		}
		return true;
	}

	void ConnectionSystem::clientDisconnect(RakNet::RakPeerInterface* peer, const char* hostAddress)
	{
		peer->CloseConnection(RakNet::SystemAddress(hostAddress), 1);
		std::cout << "Closing connection to " << RakNet::SystemAddress(hostAddress).ToString() << std::endl;
		//RakNet::AddressOrGUID(RakNet::SystemAddress(hostAddress));
	}

	// The server recognizes the connection from a client and creates an empty netID  map for that client
	void ConnectionSystem::handleConnection(GameData::GameData& data, RakNet::Packet* pack)
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

			// now add add a tank!
			std::cout << "in handle connection, sending login packet for " << *data.userName << std::endl;
			sendLoginPacket(data, *data.userName);
		}

	}

	// The server recognizes the disconnection from a client and clears all entities related to that client
	void ConnectionSystem::handleDisconnect(GameData::GameData& data, RakNet::Packet* pack)
	{
		printf("A client has disconnected. Address: %s \n", pack->systemAddress.ToString());
		for (auto const& netId : data.clientAddressToEntities[pack->systemAddress]) {
			data.m_reg.destroy(TranslationSystem::getEntity(data, netId));
			TranslationSystem::freeID(data, netId); // Free the space for that entity
		}

	}

	void ConnectionSystem::handleLostConnection(GameData::GameData& data, RakNet::Packet* pack) {
		// Currently this had the same behavior as the way we handle disconnection. TBD
		printf("A client lost the connection. Address: %s \n", pack->systemAddress.ToString());
		printf("This client has %d entities.\n", data.clientAddressToEntities[pack->systemAddress].size());
		for (auto const& netId : data.clientAddressToEntities[pack->systemAddress]) {
			std::cout << "removing entity " << netId << std::endl;
			data.m_reg.destroy(TranslationSystem::getEntity(data, netId));
			TranslationSystem::freeID(data, netId); // Free the space for that entity
		}
	}

	void ConnectionSystem::handleGameUpdate(GameData::GameData& data, RakNet::Packet* pack)
	{
		//	std::cout << "update entty packet size = " << pack->length << std::endl;
		std::string str = std::string((char*)(pack->data + 1), pack->length - 1);
		//	std::cout << "but the string of the data size = " << str.size() << std::endl;

		ReflectionSystem::ReflectionSystem reflectionSystem;
		//	std::cout << "Calling make game update " << std::endl;
		reflectionSystem.MakeGameUpdate(data, str);

	}

	void ConnectionSystem::sendLoginPacket(GameData::GameData& data, std::string& name)
	{
		std::cout << "Sending login packet for " << name << std::endl;
		MessagingSystem::MessagingSystem messagingSystem;
		RakNet::BitStream stream = RakNet::BitStream();

		messagingSystem.writeLogin(stream, name);

		data.rpi->Send(&stream,
			HIGH_PRIORITY,
			RELIABLE_ORDERED,
			0,
			data.rakAddress,
			false);
	}

	void ConnectionSystem::handleLogin(GameData::GameData& data, RakNet::Packet* pack)
	{
		MessagingSystem::MessagingSystem messagingSystem;
		UISystem::UISystem ui;
		std::string stream = std::string((char*)(pack->data + 1));
		std::string loginName = messagingSystem.readLogin(stream);
		std::cout << "Adding tank for " << loginName << std::endl;
		ui.addTank(data, loginName, pack);
	}
}