#include <vector>
#include <iostream>
#include <chrono>
#include "GameData.h"
#include "GameAdmin.h"
#include "IDTranslationComponent.h"
#include "FreeListComponent.h"
#include "RegWrappers.h"
#include "MessagingSystem.h"
#include "UISystem.h"


namespace GameAdmin {

	void MainScene::update()
	{	
		std::this_thread::sleep_for(std::chrono::milliseconds(33));
		//auto time = std::chrono::system_clock::now();
		//std::time_t formatted_time = std::chrono::system_clock::to_time_t(time);
		//std::cout << "Update systems at " << std::ctime(&formatted_time) << std::endl;
		for (PrimarySystem::PrimarySystem* system : data.primarySystemList) {
			system->update(data);
		}

	}

	MainScene::MainScene(bool isServer_, uint32_t maxClients)
	{
		// Data intialization
		data = GameData::GameData();
		data.first = true;
		data.isServer = isServer_;
		data.maxClients = maxClients;
		data.streamSize = 0;
		// Initialize our translation system, 2 way id mapping
		data.netToEnttid = std::map<networkID, entt::entity>();
		data.enttToNetidid = std::map<entt::entity, networkID>();	
		// Add the freeList component to an entity to make freeListEntity
		data.m_reg.emplace<FreeListComponent::freelist>(RegWrapper::createEntity(data.m_reg, false));


		data.primarySystemList = std::list<PrimarySystem::PrimarySystem*>();
		data.primarySystemList.push_back(&connectionSystem);
		data.primarySystemList.push_back(&movementSystem);
		data.primarySystemList.push_back(&reflectionSystem);

		for (PrimarySystem::PrimarySystem* system : data.primarySystemList)
		{
			system->init(data);
		}

		if (!isServer_) {
			
			clientLogin();
		}
		else {
			std::cout << "Starting Server" << std::endl;
			serverLogin(maxClients);
		}
	}

	void MainScene::clientLogin()
	{
		data.userName = new std::string("Player" + std::to_string(rand() % 10000));
		// Started up on socket, prompt the user to pass in a username
		//std::cout << "Please enter a username:" << std::endl;
		//std::cin >> *data.userName;
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
		delete data.userName;

	}
}