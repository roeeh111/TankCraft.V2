#include "TanksScene.h"
#include <vector>
#include <iostream>

namespace GameView {



	void TanksScene::update()
	{

		uiSystem.updateUI(m_reg, map);

	}


	void TanksScene::addClient(std::string clientName_)
	{
		auto clientEntity = m_reg.create();

		// Add the components to the the registry
		m_reg.emplace<mapObject>(clientEntity);
		m_reg.emplace<position>(clientEntity);
		m_reg.emplace<score>(clientEntity);
		m_reg.emplace<clientName>(clientEntity, clientName_);
		uiSystem.printUI(m_reg, map);

	}




	TanksScene::TanksScene(bool isServer_, uint32_t maxClients)
	{
		isServer = isServer_;
		initUISystem();
		initNetworkSystem(isServer_, maxClients);
		initIDTranslationSystem();

	}

	TanksScene::~TanksScene()
	{
		m_reg.clear();
		RakNet::RakPeerInterface::DestroyInstance(rpi);
	}


	void TanksScene::initUISystem() {
		// start by filling the matrix with blank dots
		map = std::vector<std::vector<char>>();

		//Grow rows by m
		map.resize(HEIGHT);
		for (int i = 0; i < HEIGHT; ++i)
		{
			//Grow Columns by n
			map[i].resize(WIDTH);
		}

		for (int i = 0; i < HEIGHT; i++) {
			for (int j = 0; j < WIDTH; j++) {
				if ((i + j) % 8 == 4) {
					map[i][j] = 'c';
				}
				else {
					map[i][j] = '.';
				}
			}
			std::cout << std::endl;
		}

		uiSystem.printUI(m_reg, map);
	}


	void TanksScene::initNetworkSystem(bool isServer_, uint32_t maxClients)
	{
		// Instantiate the network instance for our peer interface
		rpi = RakNet::RakPeerInterface::GetInstance();
		if (isServer) {
			// Local socket to use for communication
			RakNet::SocketDescriptor sd(SERVER_PORT, 0);

			// Startup the peer instance with our binded socket
			if (!(rpi->Startup(maxClients, &sd, 1) == RakNet::RAKNET_STARTED)) {
				std::cerr << "Failed to startup on socket!" << std::endl;
			}
			rpi->SetMaximumIncomingConnections(maxClients);

		}
		else {
			RakNet::SocketDescriptor sd;
			if (!(rpi->Startup(1, &sd, 1) == RakNet::RAKNET_STARTED)) {
				std::cerr << "Failed to startup on socket!" << std::endl;
			}
		}
	}

	void TanksScene::initIDTranslationSystem()
	{
		// Initialize our translation system
		netToEnttid = std::map<networkID, entt::entity>();
		auto freeListEntity = m_reg.create();

		// Add the components to the the registry
		m_reg.emplace<freelist> (freeListEntity);
	}
}