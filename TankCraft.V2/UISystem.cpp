#include "UISystem.h"
#include "NetworkUtilitySystem.h"
#include "IDTranslationSystem.h"
#include "Components.h"
#include "RegWrappers.h"
#include "MessagingSystem.h"

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

	// TODO: 
	void UI::addTank(GameData::GameData& data, std::string clientName_, RakNet::Packet* pack)
	{
		std::cout << "Calling addTank" << std::endl;

		// This function should be only called by the server, so it will add an entity via the network
		// and then emplace all these components. how are we putting the components on the update map?

		//auto clientEntity = RegWrapper::createEntity(data.m_reg, true);	
		auto clientEntity = NetworkUtilitySystem::addEntity(data, pack, true, true);
		std::cout << "in addTank: addEntity complete" << std::endl;

		// Add the components to the the registry
		(data.m_reg.emplace<ComponentView::mapObject>(clientEntity, true)).unlock(data, clientEntity);
		(data.m_reg.emplace<ComponentView::position>(clientEntity, true, true)).unlock(data, clientEntity);
		(data.m_reg.emplace<ComponentView::score>(clientEntity, true)).unlock(data, clientEntity);
		(data.m_reg.emplace<ComponentView::clientName>(clientEntity, clientName_, true)).unlock(data, clientEntity);
		(data.m_reg.emplace<ComponentView::userInput>(clientEntity, true)).unlock(data, clientEntity);

		std::cout << "Add tank complete, update map size = " << data.updateMap.size() << std::endl;

	}


	void UI::updateUI(GameData::GameData& data)
	{
		if (!data.isServer) {
			// get all elements that take user input
			auto view = data.m_reg.view<ComponentView::clientName, ComponentView::userInput>();


		//	std::cout << "elements in reg = " << data.m_reg.size() << std::endl;
			int i = 1;
			for (auto entity : view) {				// NOTOE: apparently there are no entities with these two components
													//(we have a clientname, not a userinput. need to network the userinput)
				// Compare clientname with our name
				std::cout << i << std::endl; i++;
				if (data.m_reg.get<ComponentView::clientName>(entity).name() == *data.userName) {
					std::cout << "Give me input: " << std::endl;
					// Get the user input for our object, only if the name matches our name
					getKeyBoardInput(data, entity);				// NOTE: Currently not being called yet since the update packet isnt being sent back
					printUI(data);
				}
			}
		}
		
		// update the map
		updateMapPositions(data);
	}

	void UI::updateMapPositions(GameData::GameData& data) {
		// get all elements that have a mapObject and position, and then do the updates
		auto view = data.m_reg.view<ComponentView::mapObject, ComponentView::position>();
		for (auto entity : view) {

			auto& pos = view.get<ComponentView::position>(entity);
			auto& disp = view.get<ComponentView::mapObject>(entity);

			// If weve met a cookie, eat it, add it to points, and remove it from the map
			if (data.map[pos.cury()][pos.curx()] == 'c') {
				// if we have a points component, increment it
				if (data.m_reg.has<ComponentView::score>(entity)) {
					// Get the entities score
					auto& scr = data.m_reg.get<ComponentView::score>(entity);
					scr.lock();
					scr.setPoints(scr.points()+1);
					scr.unlock(data, entity);
				}
			}
			data.map[pos.prevy()][pos.prevx()] = '.';
			disp.setMapChar(data.map[pos.cury()][pos.curx()]);
		}
	}

	void UI::printUI(GameData::GameData& data)
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
			std::cout << "Points for client " << view.get<ComponentView::clientName>(entity).name() << ": " << view.get<ComponentView::score>(entity).points() << std::endl;
		}
		
	}

	
	void UI::getKeyBoardInput(GameData::GameData& data, entt::entity& clientEntity)
	{
		char input;
		std::cin >> input;

		// Get the userInput component for this entity
		ComponentView::userInput& usrInput = data.m_reg.get<ComponentView::userInput>(clientEntity);
		usrInput.lock();

		usrInput.dirty_ = 1;
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

		//usrInput.unlock(data, clientEntity);			// TODO: do i really need this???

		/* TODO:																		(need a map from entity->netid for this to be quick)
		* For now, send the control to the user exactly as we inputed it.
		* For later versions, append it to a tochange queue
		*/
		NetworkUtilitySystem::sendControl(data, usrInput, TranslationSystem::getNetId(data, clientEntity)); // TODO!!!! ERROR ON THIS LINE. CRASHING NULLPOINTER 

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

// NEW USER INPUT METHOD:
// get all entities with a username
// find the entity with the players username
// call getkeyboard input on that entity
// bobs your uncle

// (change login to set the global username value)