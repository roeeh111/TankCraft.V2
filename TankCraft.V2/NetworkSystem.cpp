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
				handleConnection(data.clientAddressToEntities, pack->systemAddress);
				break;

			case ID_DISCONNECTION_NOTIFICATION:
				handleDisconnect(data.clientAddressToEntities, pack->systemAddress, data.m_reg);
				break;

			case ID_CONNECTION_LOST:
				handleLost(data.clientAddressToEntities, pack->systemAddress);
				break;

			case ID_REMOTE_NEW_INCOMING_CONNECTION:
				printf("Remote client has connected.\n");
				handleConnection(data.clientAddressToEntities, pack->systemAddress);
				break;

			case ID_REMOTE_DISCONNECTION_NOTIFICATION:
				printf("Remote client has disconnected.\n");
				handleDisconnect(data.clientAddressToEntities, pack->systemAddress, data.m_reg);
				break;

			case ID_REMOTE_CONNECTION_LOST:
				printf("Remote client has lost the connection.\n");
				handleLost(data.clientAddressToEntities, pack->systemAddress);
				break;

			case ID_NO_FREE_INCOMING_CONNECTIONS:
				printf("The server is full.\n");
				break;

			case ADD_ENTITY:
				addEntity(data, transSystem, pack, data.isServer);
			case REMOVE_ENTITY:
			case UPDATE_ENTITY:
			case CONTROL:

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
				data.message = "Our connection request has been accepted.\n";
				break;

			case ID_NO_FREE_INCOMING_CONNECTIONS:
				printf("The server is full.\n");
				break;

			case ID_DISCONNECTION_NOTIFICATION:
				printf("We have been disconnected.\n");
				break;

			case ID_CONNECTION_LOST:
				printf("Connection lost.\n");
				break;

			case ADD_ENTITY:
				addEntity(data, transSystem, pack, data.isServer);
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


	// The server recognizes the connection from a client and creates an entity map for that client
	void NetworkHandler::handleConnection(std::map<RakNet::SystemAddress, std::list<networkID>>& clientAddressToEntities, RakNet::SystemAddress& systemAddress)
	{
		printf("A client has logged in. Address: %s \n", systemAddress.ToString());

		clientAddressToEntities[systemAddress] = std::list<networkID>();
	}

	// The server recognizes the disconnection from a client and clears all entities related to that client
	void NetworkHandler::handleDisconnect(std::map<RakNet::SystemAddress, std::list<networkID>>& clientAddressToEntities, RakNet::SystemAddress& systemAddress, entt::registry& m_reg)
	{
		// TODO: how do we keep track of whose client is what entity?
		// Find all entities that are tagged with the given client's id
		// call removeEntity on all those entities
		// TODO: do we run some sort of raknet disconnect function?
		printf("A client has disconnected. Address: %s \n", systemAddress.ToString());
		for (auto const& entityid : clientAddressToEntities[systemAddress]) {
			// Add free

			m_reg.remove_if_exists(entity);									// TODO: will want to change this as the types of entities grows
		}

	}

	void NetworkHandler::handleLost(std::map<RakNet::SystemAddress, std::list<networkID>>& clientAddressToEntities, RakNet::SystemAddress& systemAddress) {
		// Do nothing yet
		printf("A client lost the connection. Address: %s \n", systemAddress.ToString());
		printf("This client has %d entities.\n", clientAddressToEntities[systemAddress].size());
	}

	// TODO:
	void NetworkHandler::addEntity(SceneComponent::SceneComponent& data, TranslationSystem::IDTranslation& transSystem, RakNet::Packet* pack, bool isServer)
	{
		// If is Server:
		if (isServer) {
			// add the entity with entity id given to the m_reg
			auto newEntity = data.m_reg.create();

			// Allocate a new netId for this entity
			networkID netid = transSystem.createMapping(data, newEntity);

			// create a new add entity packet
			Packets::addEntityPacket addpack = Packets::addEntityPacket(netid);

			data.clientAddressToEntities[pack->systemAddress]

			// broadcast to all clients to add a new entity with entity ID, and all components
			data.rpi->Send(reinterpret_cast<char*>(&addpack), 
				sizeof(addpack), 
				HIGH_PRIORITY, 
				RELIABLE_ORDERED, 
				0, 
				pack->systemAddress, 
				true);

			// TODO:			(also figure out how we want to packetize all of the components)
			// add all components of the entity with the given values to the new entity
		}
		else {
			// If is client:
			// if client doesnt have an entity with this netid 
			// add the entity with the entity id given to the m_reg
			auto newEntity = data.m_reg.create();

			// cast the input packet to an add entity packet
			Packets::addEntityPacket *addpack = (Packets::addEntityPacket*) pack->data;

			// when unpacking input packet, get the netID passed to us
			// Create a new mapping of netID and entity
			transSystem.setMapping(data, addpack->netID, newEntity);


			// TODO: 
			// add all components of the entitty with the given valies to the new entity
		}
	}

	// TODO:
	void NetworkHandler::removeEntity(SceneComponent::SceneComponent& data, TranslationSystem::IDTranslation& system, networkID netId, bool isServer)
	{
		// If is Server:
		if (isServer) {
			// Remove the given entity from the m_reg
			system.freeID(data, netId);

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
			data.m_reg.destroy(system.getEntity(data, netId));

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

