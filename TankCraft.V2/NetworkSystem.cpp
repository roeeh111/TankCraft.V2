#include <MessageIdentifiers.h>
#include "NetworkSystem.h"
#include "Packet.h"
#include "IDTranslationSystem.h"

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
				handleDisconnect(data, transSystem);
				break;

			case ID_CONNECTION_LOST:
				handleLostConnection(data, transSystem);
				break;

			case ID_REMOTE_NEW_INCOMING_CONNECTION:
				printf("Remote client has connected.\n");
				handleConnection(data, pack);
				break;

			case ID_REMOTE_DISCONNECTION_NOTIFICATION:
				printf("Remote client has disconnected.\n");
				handleDisconnect(data, transSystem);
				break;

			case ID_REMOTE_CONNECTION_LOST:
				printf("Remote client has lost the connection.\n");
				handleLostConnection(data, transSystem);
				break;

			case ID_NO_FREE_INCOMING_CONNECTIONS:
				printf("The server is full.\n");
				break;

			case ADD_ENTITY:
				std::cout << "adding entity" << std::endl;
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


	// The server recognizes the connection from a client and creates an empty netID  map for that client
	void NetworkHandler::handleConnection(SceneComponent::SceneComponent& data, RakNet::Packet* pack)
	{
		printf("A client has logged in. Address: %s \n", pack->systemAddress.ToString());

		data.clientAddressToEntities[pack->systemAddress] = std::list<networkID>();
	}

	// The server recognizes the disconnection from a client and clears all entities related to that client
	void NetworkHandler::handleDisconnect(SceneComponent::SceneComponent& data, TranslationSystem::IDTranslation& transSystem)
	{
		// TODO: how do we keep track of whose client is what entity?
		// Find all entities that are tagged with the given client's id
		// call removeEntity on all those entities
		// TODO: do we run some sort of raknet disconnect function?
		printf("A client has disconnected. Address: %s \n", data.rakAddress.ToString());
		for (auto const& netId : data.clientAddressToEntities[data.rakAddress]) {
			entt::entity entity_to_remove = transSystem.getEntity(data, netId);
			data.m_reg.remove_if_exists(entity_to_remove);
			transSystem.freeID(data, netId); // Free the space for that entity
		}

	}

	void NetworkHandler::handleLostConnection(SceneComponent::SceneComponent& data, TranslationSystem::IDTranslation& transSystem) {
		// Currently this had the same behavior as the way we handle disconnection. TBD
		printf("A client lost the connection. Address: %s \n", data.rakAddress.ToString());
		printf("This client has %d entities.\n", data.clientAddressToEntities[data.rakAddress].size());
		for (auto const& netId : data.clientAddressToEntities[data.rakAddress]) {
			data.m_reg.destroy(transSystem.getEntity(data, netId));
			transSystem.freeID(data, netId); // Free the space for that entity
		}
	}

	// TODO:
	void NetworkHandler::addEntity(SceneComponent::SceneComponent& data, TranslationSystem::IDTranslation& transSystem, RakNet::Packet* pack, bool isServer, bool initial)
	{
		// If is Server:
		if (isServer) {
			// add the entity with entity id given to the m_reg
			auto newEntity = data.m_reg.create();

			std::cout << "got entity, doing translation" << std::endl;

			// Allocate a new netId for this entity
			networkID netid = transSystem.createMapping(data, newEntity);

			std::cout << "got trans, doing new packet" << std::endl;

			// create a new add entity packet
			Packets::addEntityPacket addpack = Packets::addEntityPacket(netid);

			std::cout << "got packet, adding new address" << std::endl;

			//data.clientAddressToEntities[pack->systemAddress].push_back(netid);

			std::cout << "sending message" << std::endl;
			// broadcast to all clients to add a new entity with entity ID, and all components
			data.rpi->Send(reinterpret_cast<char*>(&addpack), 
				sizeof(addpack), 
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
			// if client doesnt have an entity with this netid 
			// add the entity with the entity id given to the m_reg

			// if were not requesting, but responding to a request
			if (!initial) {
				std::cout << "adding entity in client" << std::endl;
				auto newEntity = data.m_reg.create();

				// cast the input packet to an add entity packet
				Packets::addEntityPacket* addpack = (Packets::addEntityPacket*)pack->data;

				// when unpacking input packet, get the netID passed to us
				// Create a new mapping of netID and entity
				transSystem.setMapping(data, addpack->netID, newEntity);
			}
			else {
				// if were requesting:
				// addpacket request
				Packets::addEntityPacket addpack = Packets::addEntityPacket(0);

				// Request an addition of a new entity
				data.rpi->Send(reinterpret_cast<char*>(&addpack),
					sizeof(addpack),
					HIGH_PRIORITY,
					RELIABLE_ORDERED,
					0,
					data.rakAddress,
					false);

			}

			// TODO: 
			// add all components of the entitty with the given valies to the new entity
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

