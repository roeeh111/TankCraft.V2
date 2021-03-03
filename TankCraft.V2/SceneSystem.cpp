#include <vector>
#include <iostream>
#include <chrono>
#include "SceneSystem.h"
#include "IDTranslationComponent.h"
#include "FreeListComponent.h"


namespace SceneSystem {

	void TanksScene::update()
	{	
	//	std::chrono::seconds s = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch());

		if (data.isServer) {
			netSystem.updateServer(data, translationSystem);
		}
		else {
			netSystem.updateClient(data, translationSystem);
			//if ((s - test > std::chrono::seconds(5)) && (s - test) % 5 > std::chrono::seconds(3)) {
		//		netSystem.addEntity(data, translationSystem, nullptr, 0, 1);
			//	Sleep(5000); // I need to add this pause so that the client sends the packet once every 5 seconds
		//	}
			uiSystem.updateUI(data);
			movSystem.updateMovement(data);
			uiSystem.printUI(data);
		}
		
	}


	TanksScene::TanksScene(bool isServer_, uint32_t maxClients)
	{
		// Data
		data = SceneComponent::SceneComponent();

		test = std::chrono::duration_cast<std::chrono::seconds>(
			std::chrono::system_clock::now().time_since_epoch()
			);

		data.isServer = isServer_;
		if (!isServer_) {
			initUISystem();
			std::cout << "Starting client" << std::endl;
		}
		else {
			std::cout << "Starting Server" << std::endl;
			data.clientAddressToEntities = std::map<RakNet::SystemAddress, std::list<networkID>>();
		}

		initNetworkSystem(isServer_, maxClients);
		initIDTranslationSystem();
		initMovementSystem();
	}

	TanksScene::~TanksScene()
	{
		data.m_reg.clear();
	//	netSystem.clientDisconnect(data.rpi, data.address);
		RakNet::RakPeerInterface::DestroyInstance(data.rpi);
	}


	void TanksScene::initUISystem() {
		// start by filling the matrix with blank dots
		data.map = std::vector<std::vector<char>>();

		//Grow rows by m
		data.map.resize(HEIGHT);
		for (int i = 0; i < HEIGHT; ++i)
		{
			//Grow Columns by n
			data.map[i].resize(WIDTH);
		}

		for (int i = 0; i < HEIGHT; i++) {
			for (int j = 0; j < WIDTH; j++) {
				if ((i + j) % 8 == 4) {
					data.map[i][j] = 'c';
				}
				else {
					data.map[i][j] = '.';
				}
			}
			std::cout << std::endl;
		}

		uiSystem.printUI(data);
	}


	void TanksScene::initNetworkSystem(bool isServer_, uint32_t maxClients)
	{
		// Instantiate the network instance for our peer interface
		data.rpi = RakNet::RakPeerInterface::GetInstance();
		if (data.isServer) {
			// Local socket to use for communication
			RakNet::SocketDescriptor sd(SERVER_PORT, 0);

			// Startup the peer instance with our binded socket
			if (!(data.rpi->Startup(maxClients, &sd, 1) == RakNet::RAKNET_STARTED)) {
				std::cerr << "Failed to startup on socket!" << std::endl;
			}
			data.rpi->SetMaximumIncomingConnections(maxClients);
			printf("server started\n");
		}
		else {
			RakNet::SocketDescriptor sd;
			if (!(data.rpi->Startup(1, &sd, 1) == RakNet::RAKNET_STARTED)) {
				std::cerr << "Failed to startup on socket!" << std::endl;
			}
			else {
				data.message = "Client started\n";
				netSystem.clientConnect(data.rpi, SERVER_PORT, "127.0.0.1");
				data.rakAddress = RakNet::SystemAddress("127.0.0.1|"+ SERVER_PORT); // save the server address
			}
		}
	}

	void TanksScene::initIDTranslationSystem()
	{
		// Initialize our translation system
		data.netToEnttid = std::map<networkID, entt::entity>();
		auto freeListEntity = data.m_reg.create();

		// Add the components to the the registry
		data.m_reg.emplace<FreeListComponent::freelist> (freeListEntity);
	}

	void TanksScene::initMovementSystem()
	{
		// Empty function, nothing to do yet
	}
}