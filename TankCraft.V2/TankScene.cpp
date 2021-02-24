#include <vector>
#include <iostream>
#include "TanksScene.h"
#include "NetworkFields.h"
#include "FreeListComponent.h"

namespace GameView {



	void TanksScene::update()
	{

		systems.uiSystem.updateUI(data);
		systems.movSystem.updateMovement(data);

		// Lastly, always print out the game states
		systems.uiSystem.printUI(data);
	}


	void TanksScene::addClient(std::string clientName_)
	{
		auto clientEntity = data.m_reg.create();

		// Add the components to the the registry
		data.m_reg.emplace<ComponentView::mapObject>(clientEntity);
		data.m_reg.emplace<ComponentView::position>(clientEntity);
		data.m_reg.emplace<ComponentView::score>(clientEntity);
		data.m_reg.emplace<ComponentView::clientName>(clientEntity, clientName_);
		systems.uiSystem.printUI(data);

	}




	TanksScene::TanksScene(bool isServer_, uint32_t maxClients)
	{
		// Data
		data = SceneData::SceneData();

		// Systems
		systems = SceneSystems::SceneSystems();

		data.isServer = isServer_;
		//isServer = isServer_;
		initUISystem();
		initNetworkSystem(isServer_, maxClients);
		initIDTranslationSystem();
		initMovementSystem();

	}

	TanksScene::~TanksScene()
	{
		data.m_reg.clear();
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

		systems.uiSystem.printUI(data);
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

		}
		else {
			RakNet::SocketDescriptor sd;
			if (!(data.rpi->Startup(1, &sd, 1) == RakNet::RAKNET_STARTED)) {
				std::cerr << "Failed to startup on socket!" << std::endl;
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

	void GameView::TanksScene::initMovementSystem()
	{
		// Empty function, nothing to do yet
	}
}