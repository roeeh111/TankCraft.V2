#include "UISystem.h"
#include "NetworkUtilitySystem.h"
#include "IDTranslationSystem.h"
#include "Components.h"
#include "RegWrappers.h"
#include "MessagingSystem.h"
#include "SpikeMob.h"

/*

// On creation, add the userinput component to the client entity

user input pipeline:
user makes keyboard input
sets userinput component, and dirty bit

on network system loop, checks if user input is dirty
if it is dirty, creates new control packet, and sends it to the server

Server responds with a position component for the entity, client sets the new position value on game update

*/

namespace UISystem {

	// TODO: add into a score and health system
	void updateScore(GameData::GameData& data, entt::entity entity, ComponentView::position& pos);
	void printScore(GameData::GameData& data);
	void printHealth(GameData::GameData& data);


	void UISystem::init(GameData::GameData& data)
	{
		// Only display UI for client
		if (data.isServer) return;
		data.map = std::vector<std::vector<char>>();
		//Grow rows by m
		data.map.resize(HEIGHT);
		for (int i = 0; i < HEIGHT; ++i)
		{
			//Grow Columns by n
			data.map[i].resize(WIDTH);
		}

		// TODO: Change this to be based on what entities we have and what map positions they have, not just a static allocation
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
		printUI(data);
	}

	void UISystem::update(GameData::GameData& data)
	{
		// Only display UI for client
		if (data.isServer) { return; }
		// get all elements that take user input, and get all of the input from those entities
		auto view = data.m_reg.view<ComponentView::clientName, ComponentView::userInput>();
		for (auto entity : view) {	
			// Compare clientname with our name
			if (data.m_reg.get<ComponentView::clientName>(entity).name() == *data.userName) {
				//std::cout << "Give me input: " << std::endl;
				// Get the user input for our object, only if the name matches our name
				getKeyBoardInput(data, entity);
			//	printUI(data);
			}
		}

		// Do all movement updates
		updateMapPositions(data);
		// print the map
		printUI(data);
	}

	void UISystem::updateMapPositions(GameData::GameData& data) {
		// get all elements that have a mapObject and position, and then do the updates
		auto view = data.m_reg.view<ComponentView::mapObject, ComponentView::position>();
		int i = 0;
		//std::cout << "Updating map positions: " << std::endl;
		for (auto entity : view) {
		//	std::cout << "updating entity " << (int) entity << " with netid = " << TranslationSystem::getNetId(data, entity) << std::endl; i++;

			auto& pos = view.get<ComponentView::position>(entity);
			auto& disp = view.get<ComponentView::mapObject>(entity);

			// If weve met a cookie, eat it, add it to points, and remove it from the map
			updateScore(data, entity, pos);

			Spikes::updateSpikes(data, entity, pos);

			// If weve met spikes, cause damage to us and remove the spikes

			// Set the previous location to a "." and move on
			data.map[pos.prevy()][pos.prevx()] = '.';
			data.map[pos.cury()][pos.curx()] = disp.mapChar();
		//	disp.setMapChar(data.map[pos.cury()][pos.curx()]);
		}
		std::cout << std::endl;
	}

	void UISystem::printUI(GameData::GameData& data)
	{
		// Clear the current screen
		
		system("CLS");

		// Print out whatever is in the map
		std::cout << std::endl;
		for (int i = 0; i < HEIGHT; i++) {
			for (int j = 0; j < WIDTH; j++) {
				std::cout << data.map[i][j] << " ";
			}
			std::cout << std::endl;
		}

		// Print out points and client names of any entities that have those
		printScore(data);
		printHealth(data);
	}

	void UISystem::getKeyBoardInput(GameData::GameData& data, entt::entity& clientEntity)
	{

		// Get the userInput component for this entity
		ComponentView::userInput& usrInput = data.m_reg.get<ComponentView::userInput>(clientEntity);
		
		// Clear the input
		usrInput.clear();

		usrInput.dirty_ = 1;

		/*
		char input;
		std::cin >> input;
		// set the user input values depending on what we got
		if (input == 'a') {
			usrInput.setLeft(true);
		}
		else if (input == 's') {
			usrInput.setDown(true);
		}
		else if (input == 'd') {
			usrInput.setRight(true);
		}
		else if (input == 'w') {
			usrInput.setUp(true);
		}
		else {
			usrInput.dirty_ = 0;
		}
		*/
		if (GetAsyncKeyState(87)) {
			usrInput.setUp(true);
		}
		else if (GetAsyncKeyState(65)) {
			usrInput.setLeft(true);
		}
		else if (GetAsyncKeyState(83)) {
			usrInput.setDown(true);
		}
		else if (GetAsyncKeyState(68)) {
			usrInput.setRight(true);
		}
		else {
			usrInput.dirty_ = 0;
		}
		//std::cout << "left = " << usrInput.left() << " right = " << usrInput.right() << " up = " << usrInput.up() << " down = " << usrInput.down() << std::endl;


		//usrInput.unlock(data, clientEntity);			// TODO: do i really need this???

		/* TODO:																		(need a map from entity->netid for this to be quick)
		* For now, send the control to the user exactly as we inputed it.
		* For later versions, append it to a tochange queue
		*/
		NetworkUtilitySystem::sendControl(data, usrInput, TranslationSystem::getNetId(data, clientEntity)); // TODO!!!! ERROR ON THIS LINE. CRASHING NULLPOINTER 
	}


	void updateScore(GameData::GameData& data, entt::entity entity, ComponentView::position& pos)
	{
		if (data.map[pos.cury()][pos.curx()] == 'c') {

			if (data.m_reg.has<ComponentView::score>(entity)) {
				// Get the entities score
				auto& scr = data.m_reg.get<ComponentView::score>(entity);
				scr.lock();
				scr.setPoints(scr.points() + 1);
				scr.unlock(data, entity);
			}
		}
	}

	void printScore(GameData::GameData& data)
	{
		// Print out points and client names of any entities that have those
		auto view = data.m_reg.view<ComponentView::score, ComponentView::clientName>();
		for (auto entity : view) {
			std::cout << "Points for client " << view.get<ComponentView::clientName>(entity).name() << ": " << view.get<ComponentView::score>(entity).points() << std::endl;
		}

	}

	void printHealth(GameData::GameData& data)
	{
		auto view = data.m_reg.view<ComponentView::health, ComponentView::clientName>();
		for (auto entity : view) {
			std::cout << "Health for client " << view.get<ComponentView::clientName>(entity).name() << ": " << view.get<ComponentView::health>(entity).hp() << std::endl;
		}
	}


}

