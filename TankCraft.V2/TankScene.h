#pragma once
#include <list>
#include <entt/entt.hpp>
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

		void addSpikes(position pos);

		void getUserInput(entt::entity& clientEntity);

		TanksScene();
		~TanksScene();

	private:

		char map[HEIGHT][WIDTH]; // do we make this component based? is this in the server component?

		// main registry
		entt::registry m_reg;
	};
}


