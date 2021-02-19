#pragma once
//#include <list>
#include <entt/entt.hpp>
#include <RakPeerInterface.h>
#include "TankObjects.h"

#define WIDTH 20
#define HEIGHT 20

namespace Tanks {

	class TanksScene {
	public:
		// print out the current game map to console
		void printUI();

		// Scan through each client, check if its dirty bit is set and change data if it is
		void update();

		void addTank(std::string clientName_);

		void addSpikes(struct position pos);

		void getUserInput(entt::entity& clientEntity);

		TanksScene();
		~TanksScene();

	private:

		// The main UI
		char map[HEIGHT][WIDTH];

		// main registry
		entt::registry m_reg;

		// The interface with raknet 
		RakNet::RakPeerInterface* rpi;

		// Whether this scene is the server or a client 
		bool isServer; 
	};
}


