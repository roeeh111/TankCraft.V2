#include "NetworkHandler.h"

/**
* 
* WE WILL DELETE THIS FILE ONCE ALL NECCESARRY CODE IS TRANSFRED TO NETWORKSYSTEM.CPP
* 
* THIS IS SHIT, DELETE IT!!!!!
* 
*/





namespace Networking {
	// Constructor for local client/server instantiation
	NetworkHandler::NetworkHandler(bool isServer_)
	{
		port = SERVER_PORT;
		hostAddress = "127.0.0.1";
		isServer = isServer_;
		packet = nullptr;

		if (isServer) {
			// instatiate socket
			sd = RakNet::SocketDescriptor(port, hostAddress);
			maxClients = MAX_CLIENTS;

			// startup peer
			if (!(peer->Startup(MAX_CLIENTS, &sd, 1) == RakNet::RAKNET_STARTED)) {
				std::cerr << "Failed to startup on socket!" << std::endl;
			}

			// instantiating the server side of things, if were a server, we just set incomming connections, security, authentication etc..
			// TODO: mess with the settings of how we want our server to act like
			peer->SetMaximumIncomingConnections(MAX_CLIENTS);
			peer->DisableSecurity();
			std::cout << "Starting the Server!" << std::endl;
		}
		else {
			// not a server component, so try and connect to the host 
			maxClients = 1;

			if (!(peer->Startup(1, &sd, 1) == RakNet::RAKNET_STARTED)) {
				std::cerr << "Failed to startup on socket!" << std::endl;
			}
			if (!(peer->Connect(hostAddress, port, 0, 0) == RakNet::CONNECTION_ATTEMPT_STARTED)) {
				std::cerr << "Connection attempt failed" << std::endl;
			}

			std::cout << "Starting up a client" << std::endl;
		}
	}
	NetworkHandler::NetworkHandler(bool isServer_, unsigned short port_, const char* hostAddress_, unsigned int maxClients_)
	{
		port = port_;
		hostAddress = hostAddress_;
		isServer = isServer_;
		maxClients = maxClients_;
		packet = nullptr;

		if (isServer) {
			// instatiate socket
			sd = RakNet::SocketDescriptor(port, hostAddress);

			// startup peer
			if (!(peer->Startup(MAX_CLIENTS, &sd, 1) == RakNet::RAKNET_STARTED)) {
				std::cerr << "Failed to startup on socket!" << std::endl;
			}

			maxClients = MAX_CLIENTS;

			// instantiating the server side of things, if were a server, we just set incomming connections, security, authentication etc..
			// TODO: mess with the settings of how we want our server to act like
			peer->SetMaximumIncomingConnections(MAX_CLIENTS);
			peer->DisableSecurity();
			std::cout << "Starting the Server!" << std::endl;
		}
		else {
			// not a server component, so try and connect to the host 
			maxClients = 1;
			if (!(peer->Startup(1, &sd, 1) == RakNet::RAKNET_STARTED)) {
				std::cerr << "Failed to startup on socket!" << std::endl;
			}
			if (!(peer->Connect(hostAddress, port, 0, 0) == RakNet::CONNECTION_ATTEMPT_STARTED)) {
				std::cerr << "Connection attempt failed" << std::endl;
			}
			std::cout << "Starting up a client" << std::endl;
		}
	}

}
