#include <vector>
#include <iostream>
#include <chrono>
#include "GameAdmin.h"
#include "IDTranslationComponent.h"
#include "FreeListComponent.h"
#include "CreateEntity.h"


namespace GameAdmin {

	void TanksScene::update()
	{	

		if (data.isServer) {
			NetworkSystem::updateServer(data);
		//	UI::updateUI(data);
		//	UI::printUI(data);
		}
		else {
			NetworkSystem::updateClient(data);
			if (data.first) {
				NetworkSystem::addEntity(data, nullptr, 0, 0);
				data.first = false;
			}

			UI::updateUI(data);
			MovementSystem::updateMovement(data);
			UI::printUI(data);
		}
		
	}

	void TanksScene::clientLogin()
	{
		initUISystem();
		initNetworkSystem(data.isServer, 1);
		initIDTranslationSystem(false); // TODO: change this 
		initMovementSystem();

		// Started up on socket, prompt the user to pass in a username
		std::string uinput;
		std::cout << "Please enter a username:" << std::endl;
		std::cin >> uinput;

		// create a new tank entity with that username, call network add entity and update entity (or put on update queue)
		UI::addTank(data, uinput);
	}

	void TanksScene::serverLogin(uint32_t maxClients) {
		data.clientAddressToEntities = std::map<RakNet::SystemAddress, std::list<networkID>>();
		initNetworkSystem(data.isServer, maxClients);
		initIDTranslationSystem(true); // TODO: change this 
		initMovementSystem();
	}

	TanksScene::TanksScene(bool isServer_, uint32_t maxClients)
	{
		// Data
		data = GameData::GameData();
		data.first = true;

		// for debug
		test = std::chrono::duration_cast<std::chrono::seconds>(
			std::chrono::system_clock::now().time_since_epoch()
			);

		data.isServer = isServer_;


		if (!isServer_) {
			std::cout << "Starting client" << std::endl;
			clientLogin();
		}
		else {
			std::cout << "Starting Server" << std::endl;
			serverLogin(maxClients);
		}
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

		UI::printUI(data);
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
				NetworkSystem::clientConnect(data.rpi, SERVER_PORT, "127.0.0.1");
				data.rakAddress = RakNet::SystemAddress("127.0.0.1|"+ SERVER_PORT); // save the server address
			}
		}
	}

	void TanksScene::initIDTranslationSystem(bool isServer) // right now, both the client and the server have a freelist entity, do they need that?
	{
		// Initialize our translation system
		data.netToEnttid = std::map<networkID, entt::entity>();
		//auto freeListEntity = data.m_reg.create();		
		auto freeListEntity = RegWrapper::createEntity(data.m_reg, false);			// Only other time an entity is created, 


		// Add the components to the the registry
		data.m_reg.emplace<FreeListComponent::freelist> (freeListEntity);
	}

	void TanksScene::initMovementSystem()
	{
		// Empty function, nothing to do yet
	}

	// Initialize the outgoing game update message
	void TanksScene::initMessageSystem()
	{
		// initialize the update map
		data.updateMap = std::map<networkID, std::list<baseComponent>>();

	}
}