#pragma once
#include <iostream>
#include <RakPeer.h>
#include <MessageIdentifiers.h>
#include "RakPeerInterface.h"

// TODO: add send method (integrate with serializatoin)

// TODO: add receive and dispatch/update method


#define MAX_CLIENTS 10
#define SERVER_PORT 60000

namespace Networking {
	class NetworkHandler {
	public:

		// Default constructor, essentially just connect to the given server address
		NetworkHandler() {
			port = SERVER_PORT;
			hostAddress = "127.0.0.1";
			maxClients = 1;
			isServer = false;
			packet = nullptr;

			if (!(peer->Startup(1, &sd, 1) == RakNet::RAKNET_STARTED)) {
				std::cerr << "Failed to startup on socket!" << std::endl;
			}
			if (!(peer->Connect(hostAddress, port, 0, 0) == RakNet::CONNECTION_ATTEMPT_STARTED)) {
				std::cerr << "Connection attempt failed" << std::endl;
			}
			std::cout << "Starting up a default client" << std::endl;
		}

		// Constructor to instantiate local client or server handler
		NetworkHandler(bool isServer_);

		// Deep constructor
		NetworkHandler(bool isServer_, unsigned short port_, const char* hostAddress_, unsigned int maxClients_);

		~NetworkHandler() {
			RakNet::RakPeerInterface::DestroyInstance(peer);
		}
		
		// Send a serialized copy of the component to a destination
		template<typename type>
		void send(type component, RakNet::SystemAddress addr);

		// Send a serialized copy of the component to all connections
		template<typename type>
		void broadcast(type component);

		bool getIsServer() const { return isServer; }

	private:
		// Local socket to use for communication
		RakNet::SocketDescriptor sd;

		// Main instance for network communication, holds current network state data
		RakNet::RakPeerInterface* peer;

		// Packet Iterating pointer
		RakNet::Packet* packet;

		// For now keep track of the port and max clients as a cache:
		unsigned short port;
		const char* hostAddress;
		unsigned int maxClients;
		bool isServer;
	};

	// Send a serialized copy of the component to a destination
	template<typename type>
	inline void NetworkHandler::send(type component, RakNet::SystemAddress addr)
	{
		// Our outputting bitstream
		RakNet::BitStream bsOut;
		
		// In header, say that this is a standard game address
		bsOut.Write((RakNet::MessageID)ID_GAME_MESSAGE_1);

																						//	TODO:: Do we need to add anything to the header fields?? entity number??   

		// Write the serialized component to the bitstream
		Serialization::write<type>(bsOut, component);

		peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, addr, false);		 // TODO:: how do we get the system address??? which packet?????
	}
	template<typename type>
	inline void NetworkHandler::broadcast(type component)
	{
		// Our outputting bitstream
		RakNet::BitStream bsOut;

		// In header, say that this is a standard game address
		bsOut.Write((RakNet::MessageID)ID_GAME_MESSAGE_1);

																				//	TODO:: Do we need to add anything to the header fields?? entity number??   

		// Write the serialized component to the bitstream
		Serialization::write<type>(bsOut, component);

		peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, 0, true);						// TODO:: how do we get the system address??? which packet?????

	}
}