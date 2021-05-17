#include "UISystem.h"
#include "NetworkUtilitySystem.h"
#include "IDTranslationSystem.h"
#include "Components.h"
#include "RegWrappers.h"
#include "MessagingSystem.h"
#include "SpikeMob.h"
#include "ScoreSystem.h"
#include "HealthAndDamageSystem.h"


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

		for (int i = 0; i < HEIGHT; i++) {
			for (int j = 0; j < WIDTH; j++) {
				data.map[i][j] = '.';
			}
			std::cout << std::endl;
		}
		printUI(data);
	}

	void UISystem::update(GameData::GameData& data)
	{
		// Only display UI for client
		if (data.isServer) { return; }
		bool input = false;

		// get all elements that take user input, and get all of the input from those entities
		auto view = data.m_reg.view<ComponentView::clientName, ComponentView::userInput>();
		for (auto entity : view) {	
			// Compare clientname with our name
			if (view.get<ComponentView::clientName>(entity).name() == *data.userName) {
				// Get the user input for our object, only if the name matches our name
				input = getKeyBoardInput(data, entity);
			//	printUI(data);
			}
		}

		// Do all movement updates
		updateMapPositions(data);
		// print the map
		if (input)
			printUI(data);
	}

	void UISystem::updateMapPositions(GameData::GameData& data) {
		// get all elements that have a mapObject and position, and then do the updates
		auto view = data.m_reg.view<ComponentView::mapObject, ComponentView::position>();
		MovementSystem::moveMobs(data);

		for (auto entity : view) {

			auto& pos = view.get<ComponentView::position>(entity);
			auto& disp = view.get<ComponentView::mapObject>(entity);

			// Update any score interaction damage interaction, or mob movement
			ScoreSystem::updateScore(data, entity, pos);
			HealthAndDamageSystem::updateDamage(data, entity, pos);
			// Set the previous location to a "." and move on
			data.map[pos.prevy()][pos.prevx()] = '.';
			data.map[pos.cury()][pos.curx()] = disp.mapChar();
		}
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
		ScoreSystem::printScore(data);
		HealthAndDamageSystem::printHealth(data);
		
	}

	bool UISystem::getKeyBoardInput(GameData::GameData& data, entt::entity& clientEntity)
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
		}*/
		
		// Commenting out for debug
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

		// Only send input if dirty
		if (usrInput.dirty_) {
			std::cout << "Sending control for netid = " << TranslationSystem::getNetId(data, clientEntity) << std::endl;
			NetworkUtilitySystem::sendControl(data, usrInput, TranslationSystem::getNetId(data, clientEntity));
			return true;
		}
		return false;
	}

}

