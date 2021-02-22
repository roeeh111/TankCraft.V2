#include "TankScene.h"
#include <vector>
#include <iostream>

namespace GameView {
	void TanksScene::printUI()
	{
		system("CLS");
		std::cout << std::endl;
		for (int i = 0; i < HEIGHT; i++) {
			for (int j = 0; j < WIDTH; j++) {
				std::cout << map[i][j] << " ";
			}
			std::cout << std::endl;
		}

		int i = 0;
		auto view = m_reg.view<score, clientName>();
		for (auto entity : view) {
			std::cout << "Points for client " << view.get<clientName>(entity).name << ": " << view.get<score>(entity).points << std::endl;
			i++;
		}

	}


	// TODO: different versions of update if the current running program is a server or a client (do isserver check from user on startup)
	void TanksScene::update()
	{

		auto view = m_reg.view<dirtyClient, position, score, mapObject>();

		// TODO: get all elements that have a mapObject, and then do the updates



		for (auto entity : view) {
			TanksScene::getUserInput(entity);// TODO: change this to some networking transaction??????

			auto& d = view.get<dirtyClient>(entity);
			auto& pos = view.get<position>(entity);
			auto& scr = view.get<score>(entity);
			auto& disp = view.get<mapObject>(entity);


			if (d.dirty) { //												TODO: change this to only when a client packet with an object came in (if entity has position)
				std::cout << "dirty bit is set, reprinting" << std::endl;

				// If the dirty bit is set, set old location to '.', and 
				// set new location to what it needs to be. also set dirty bit to 0
				map[pos.prevy][pos.prevx] = '.';

				// If weve met a cookie, eat it, add it to points, and remove it from the map
				if (map[pos.cury][pos.curx] == 'c') {
					scr.points++;
				}

				map[pos.cury][pos.curx] = disp.mapChar;

				d.dirty = 0;

				// Since we only have 1 client and that is us, we print. (in the future we wont print, but respond back over the network)
			}
		}
		TanksScene::printUI();

	}


	void TanksScene::addClient(std::string clientName_)
	{
		auto clientEntity = m_reg.create();

		// Add the components to the the registry
		m_reg.emplace<mapObject>(clientEntity);
		m_reg.emplace<position>(clientEntity);
		m_reg.emplace<score>(clientEntity);
		m_reg.emplace<dirtyClient>(clientEntity);
		m_reg.emplace<clientName>(clientEntity, clientName_);
	}
	/*
	void TanksScene::addSpikes(position pos) {
		auto clientEntity = m_reg.create();
		//	m_reg.emplace<position>(clientEntity, pos);
		m_reg.emplace<mapObject>(clientEntity, 'b');
	}
	*/



	TanksScene::TanksScene(bool isServer_, uint32_t maxClients)
	{
		isServer = isServer_;
		// start by filling the matrix with blank dots
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
		printUI();

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

		// Initialize our translation system
		netToEnttid = std::map<networkID, entt::entity>();

	}

	TanksScene::~TanksScene()
	{
		m_reg.clear();
		RakNet::RakPeerInterface::DestroyInstance(rpi);
	}


	void TanksScene::getUserInput(entt::entity& clientEntity)
	{
		std::cout << "Getting user input" << std::endl;

		char input;
		auto& dirty = m_reg.get<dirtyClient>(clientEntity);
		auto& points = m_reg.get<position>(clientEntity);

		//userStream.input.get(&input, 1); //>> input; // TODO:: fix this stream issue (decide how we are getting input from over the network for state data first)

		std::cin >> input;

		dirty.dirty = 1;
		points.prevx = points.curx;
		points.prevy = points.cury;

		if (input == 'a') {
			if (points.curx == 0) {
				points.curx = WIDTH - 1;
			}
			else {
				points.curx--;
			}
		}
		else if (input == 's') {
			if (points.cury == HEIGHT - 1) {
				points.cury = 0;
			}
			else {
				points.cury++;
			}
		}
		else if (input == 'd') {
			if (points.curx == WIDTH - 1) {
				points.curx = 0;
			}
			else {
				points.curx++;
			}
		}
		else if (input == 'w') {
			if (points.cury == 0) {
				points.cury = HEIGHT - 1;
			}
			else {
				points.cury--;
			}
		}
		else {
			std::cout << "setting dirty = 0" << std::endl;
			dirty.dirty = 0;
		}
	}

	/*
	TanksScene::TanksScene()
	{
		// start by filling the matrix with blank dots
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
		printUI();

		// Instantiate the network instance for our peer interface
		rpi = RakNet::RakPeerInterface::GetInstance();

		if (!(rpi->Startup(1, &sd, 1) == RakNet::RAKNET_STARTED)) {
			std::cerr << "Failed to startup on socket!" << std::endl;
		}
	}
	*/

}