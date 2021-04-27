#include <vector>
#include <iostream>
#include <chrono>
#include "GameData.h"
#include "GameAdmin.h"
#include "IDTranslationComponent.h"
#include "FreeListComponent.h"
#include "RegWrappers.h"
#include "MessagingSystem.h"


namespace GameAdmin {

	void MainScene::update()
	{	
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
		//Please help me fix this linker error, or uncomment lines below to get things to work
		for (PrimarySystem::PrimarySystem& system : data.primarySystemList)
		{
			system.update(data);
		}
		//connectionSystem.update(data);
		//movementSystem.update(data);
		//messagingSystem.update(data);
		//ui.update(data);
	}

	MainScene::MainScene(bool isServer_, uint32_t maxClients)
	{
		// Data intialization
		data = GameData::GameData();
		data.first = true;
		data.isServer = isServer_;
		data.maxClients = 10;
		// Initialize our translation system, 2 way id mapping
		data.netToEnttid = std::map<networkID, entt::entity>();
		data.enttToNetidid = std::map<entt::entity, networkID>();	
		// Add the freeList component to an entity to make freeListEntity
		data.m_reg.emplace<FreeListComponent::freelist>(RegWrapper::createEntity(data.m_reg, false));

		data.primarySystemList = std::list<PrimarySystem::PrimarySystem>();
		data.primarySystemList.push_back(connectionSystem);
		data.primarySystemList.push_back(movementSystem);
		data.primarySystemList.push_back(messagingSystem);
		data.primarySystemList.push_back(ui);

		//connectionSystem.init(data);
		//movementSystem.init(data);
		//messagingSystem.init(data);
		//ui.init(data);
		for (PrimarySystem::PrimarySystem& system : data.primarySystemList)
		{
			system.init(data);
		}

		if (!isServer_) {
			std::cout << "Starting client" << std::endl;
			clientLogin();
		}
		else {
			std::cout << "Starting Server" << std::endl;
			serverLogin(maxClients);
		}
	}

	void MainScene::clientLogin()
	{
		data.userName = new std::string();
		// Started up on socket, prompt the user to pass in a username
		std::cout << "Please enter a username:" << std::endl;
		std::cin >> *data.userName;
		// create a new tank entity with that username, call network add entity and update entity (or put on update queue)
		connectionSystem.sendLoginPacket(data, *data.userName);
	}

	void MainScene::serverLogin(uint32_t maxClients) {
		data.clientAddressToEntities = std::map<RakNet::SystemAddress, std::list<networkID>>();
	}

	MainScene::~MainScene()
	{
		data.m_reg.clear();
		RakNet::RakPeerInterface::DestroyInstance(data.rpi);
	}
}