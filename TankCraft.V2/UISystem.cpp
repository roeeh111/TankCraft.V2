#include "UISystem.h"
#include "NetworkSystem.h"
#include "Components.h"
#include "CreateEntity.h"

/*

// On creation, add the userinput component to the client entity

user input pipeline:
user makes keyboard input
sets userinput component, and dirty bit

on network system loop, checks if user input is dirty
if it is dirty, creates new control packet, and sends it to the server

Server responds with a position component for the entity, client sets the new position value on game update

*/

namespace UI {

	//TODO: Take out the registry wrting and call network system's add entity function
	void UISystem::addTank(GameData::GameData& data, std::string clientName_)
	{
	//	auto clientEntity = data.m_reg.create(); 
		auto clientEntity = RegWrapper::createEntity(data.m_reg, true);

		// Add the components to the the registry
		data.m_reg.emplace<ComponentView::mapObject>(clientEntity);
		data.m_reg.emplace<ComponentView::position>(clientEntity, 1);
		data.m_reg.emplace<ComponentView::score>(clientEntity);
		data.m_reg.emplace<ComponentView::clientName>(clientEntity, clientName_);
		data.m_reg.emplace<ComponentView::userInput>(clientEntity);
		printUI(data);
	}


	void UISystem::updateUI(GameData::GameData& data)
	{
		/*
		// get all elements that take user input
		auto view = data.m_reg.view<ComponentView::userInput>();
		for (auto entity : view) {
			// Get the user input for our object
			getKeyBoardInput(data.m_reg, entity);
			//getUserInput(data.m_reg, entity);
		}
		*/
		// update the map
		updateMapPositions(data);
	}

	void UISystem::updateMapPositions(GameData::GameData& data) {
		// get all elements that have a mapObject and position, and then do the updates
		auto view = data.m_reg.view<ComponentView::mapObject, ComponentView::position>();
		for (auto entity : view) {

			auto& pos = view.get<ComponentView::position>(entity);
			auto& disp = view.get<ComponentView::mapObject>(entity);

			// If weve met a cookie, eat it, add it to points, and remove it from the map
			if (data.map[pos.cury][pos.curx] == 'c') {
				// if we have a points component, increment it
				if (data.m_reg.has<ComponentView::score>(entity)) {
					// Get the entities score
					auto& scr = data.m_reg.get<ComponentView::score>(entity);
					scr.points++;
				}
			}
			data.map[pos.prevy][pos.prevx] = '.';
			data.map[pos.cury][pos.curx] = disp.mapChar;
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
		auto view = data.m_reg.view<ComponentView::score, ComponentView::clientName>();
		for (auto entity : view) {
			std::cout << "Points for client " << view.get<ComponentView::clientName>(entity).name << ": " << view.get<ComponentView::score>(entity).points << std::endl;
		}
		std::cout << data.message << std::endl;
	}

	
	void UISystem::getKeyBoardInput(entt::registry& m_reg, entt::entity& clientEntity)
	{
		char input;
		std::cin >> input;

		// Get the userInput component for this entity
		ComponentView::userInput& usrInput = m_reg.get<ComponentView::userInput>(clientEntity);

		usrInput.dirty = 1;
		// set the user input values depending on what we got
		if (input == 'a') {
			usrInput.left = 1;
		}
		else if (input == 's') {
			usrInput.down = 1;
		}
		else if (input == 'd') {
			usrInput.right = 1;
		}
		else if (input == 'w') {
			usrInput.up = 1;
		}
		else {
			usrInput.dirty = 0;
		}
	}
	

}







/*
void UISystem::getUserInput(entt::registry& m_reg, entt::entity& clientEntity)
{

	char input;
	auto& points = m_reg.get<ComponentView::position>(clientEntity);


	std::cin >> input;

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

}
*/


