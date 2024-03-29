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

	void UISystem::initialize(GameData::GameData& data)
	{
		data.map = std::vector<std::vector<int>>(HEIGHT, std::vector<int>(WIDTH));
		data.map = {
			{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1},
			{1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1},
			{1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1},
			{1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
			{1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		};
	}

	void UISystem::updateClient(GameData::GameData& data, HDC hdcBackBuff, PAINTSTRUCT ps, HWND hwnd)
	{
		data.hdcBackBuff = hdcBackBuff;
		data.ps = ps;
		data.hwnd = hwnd;
		HPEN RedPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		static HBRUSH WhiteBrush = CreateSolidBrush(RGB(255, 255, 255));
		static HBRUSH BlackBrush = CreateSolidBrush(RGB(0, 0, 0));
		static HBRUSH YellowBrush = CreateSolidBrush(RGB(255, 255, 0));
		static HBRUSH GreenBrush = CreateSolidBrush(RGB(173, 255, 47));
		static HBRUSH RedBrush = CreateSolidBrush(RGB(255, 0, 0));
		static HBRUSH ZombieBrush = CreateSolidBrush(RGB(0, 102, 0));
		static HBRUSH BlueBrush = CreateSolidBrush(RGB(0, 0, 255));
		char buf[256];
		bool input = false;
		// get all elements that take user input, and get all of the input from those entities
		auto view = data.m_reg.view<ComponentView::clientName, ComponentView::userInput>();
		for (auto entity : view) {
			// Compare clientname with our name
			// Get the user input for our object, only if the name matches our name
			if (view.get<ComponentView::clientName>(entity).name() == *data.userName) {
				// std::cout << "Give me input for : " << view.get<ComponentView::clientName>(entity).name() << std::endl;
				if (GetForegroundWindow() == data.hwnd) {
					input = getKeyBoardInput(data, entity);
				}
			}
		}

		//draw GameMap
		int i = 0, j = 0;
		for (i = 0; i < WIDTH; i++)
		{
			for (j = 0; j < HEIGHT; j++)
			{
				if (data.map[j][i]) 
				{
					if (data.map[j][i] == 1)
					{
						SelectObject(hdcBackBuff, BlackBrush);
						Rectangle(hdcBackBuff, i * 16, j * 16, (i * 16) + 15, (j * 16) + 15);
					}
				}
			}
		}

		////////////////////////UPDATE MAP POSITIONS////////////////////////
		auto pos_view = data.m_reg.view<ComponentView::mapObject, ComponentView::position>();
	//	MovementSystem::moveMobs(data);

		int offset = 0;
		int playernum = 1;
		for (auto entity : pos_view) {
			//	std::cout << "updating entity " << (int) entity << " with netid = " << TranslationSystem::getNetId(data, entity) << std::endl;
			auto& pos = pos_view.get<ComponentView::position>(entity);
			auto& disp = pos_view.get<ComponentView::mapObject>(entity);

			// Update any score interaction damage interaction, or mob movement
			ScoreSystem::updateScore(data, entity, pos);
			HealthAndDamageSystem::updateDamage(data, entity, pos);

			data.map[pos.prevy()][pos.prevx()] = 0;
			if (disp.mapChar() == 'X')
			{
				SelectObject(hdcBackBuff, GreenBrush);
				RoundRect(hdcBackBuff, pos.curx() * 16, pos.cury() * 16, (pos.curx() * 16) + 15, (pos.cury() * 16) + 15, 5, 5);
				sprintf(&buf[0], "%d", playernum);
				TextOut(hdcBackBuff, pos.curx() * 16, pos.cury() * 16, buf, strlen(buf));
				SetTextColor(hdcBackBuff, RGB(0, 0, 255));
				if (data.m_reg.has<ComponentView::clientName>(entity)) {
					auto usrname = data.m_reg.get<ComponentView::clientName>(entity).name();
					sprintf(&buf[0], "Player Name: player %d", playernum);
					TextOut(hdcBackBuff, 350, 24 + offset, buf, strlen(buf));
				}

				// Get health, coins and score
				if (data.m_reg.has<ComponentView::health>(entity)) {
					auto& hlth = data.m_reg.get<ComponentView::health>(entity);
					sprintf(&buf[0], "Health: %d", hlth.hp());
					TextOut(hdcBackBuff, 350, 48 + offset, buf, strlen(buf));
				}

				if (data.m_reg.has<ComponentView::score>(entity)) {
					auto& scr = data.m_reg.get<ComponentView::score>(entity);
					sprintf(&buf[0], "Score: %d", scr.points());
					TextOut(hdcBackBuff, 350, 72 + offset, buf, strlen(buf));
				}
				offset += 84;
				playernum++;
			}
			else if (disp.mapChar() == 'S') {
				data.map[pos.cury()][pos.curx()] = 'S';
				SelectObject(hdcBackBuff, RedBrush);
				Rectangle(hdcBackBuff, pos.curx() * 16, pos.cury() * 16, (pos.curx() * 16) + 15, (pos.cury() * 16) + 15);
			}
			else if (disp.mapChar() == 'C') {
				data.map[pos.cury()][pos.curx()] = 'C';
				SelectObject(hdcBackBuff, YellowBrush);
				Ellipse(hdcBackBuff, pos.curx() * 16, pos.cury() * 16, (pos.curx() * 16) + 15, (pos.cury() * 16) + 15);

			}
			else if (disp.mapChar() == 'Z') {
				data.map[pos.cury()][pos.curx()] = 'Z';
				SelectObject(hdcBackBuff, ZombieBrush);
				RoundRect(hdcBackBuff, pos.curx() * 16, pos.cury() * 16, (pos.curx() * 16) + 15, (pos.cury() * 16) + 15, 5, 5);
			}
		}
		////////////////////////UPDATE MAP POSITIONS////////////////////////


		auto view1 = data.m_reg.view<ComponentView::mapObject, ComponentView::position, ComponentView::pointsGiven>();
	
		sprintf(&buf[0], "coins: %d", view1.size_hint());
		TextOut(hdcBackBuff, 350, 72 + offset, buf, strlen(buf));

		BitBlt(ps.hdc, 0, 0, 640, 480, hdcBackBuff, 0, 0, SRCCOPY);

		EndPaint(hwnd, &ps);
	}

	void UISystem::updateServer(GameData::GameData& data, HDC hdcBackBuff, PAINTSTRUCT ps, HWND hwnd) {
		data.hdcBackBuff = hdcBackBuff;
		data.ps = ps;
		data.hwnd = hwnd;
		char buf[256];
		int offset = 50;
		int offsetIncr = 17;
		SetTextColor(hdcBackBuff, RGB(0, 0, 255));
		sprintf(&buf[0], "Server started");
		TextOut(hdcBackBuff, 50, offset, buf, strlen(buf));
		offset += offsetIncr;
		sprintf(&buf[0], "Current Stream Size: %d", data.streamSize);
		TextOut(hdcBackBuff, 50, offset, buf, strlen(buf));
		auto view = data.m_reg.view<ComponentView::mapObject, ComponentView::position>();
		for (auto entity : view) {
			offset += offsetIncr;
			auto& pos = view.get<ComponentView::position>(entity);
			auto& disp = view.get<ComponentView::mapObject>(entity);
			if (disp.mapChar() == 'X')
			{
				sprintf(&buf[0], "Player entity at (%d, %d)", pos.curx(), pos.cury());
				TextOut(hdcBackBuff, 50, offset, buf, strlen(buf));
			}
			else if (disp.mapChar() == 'S') {
				sprintf(&buf[0], "Spike entity at (%d, %d)", pos.curx(), pos.cury());
				TextOut(hdcBackBuff, 50, offset, buf, strlen(buf));
			}
			else if (disp.mapChar() == 'C') {
				sprintf(&buf[0], "Coin entity at (%d, %d)", pos.curx(), pos.cury());
				TextOut(hdcBackBuff, 50, offset, buf, strlen(buf));
			}
			else if (disp.mapChar() == 'Z') {
				sprintf(&buf[0], "Zombie entity at (%d, %d)", pos.curx(), pos.cury());
				TextOut(hdcBackBuff, 50, offset, buf, strlen(buf));
			}
		}

		BitBlt(ps.hdc, 0, 0, 640, 480, hdcBackBuff, 0, 0, SRCCOPY);
		EndPaint(hwnd, &ps);
		MovementSystem::moveMobs(data);

	}

	bool UISystem::getKeyBoardInput(GameData::GameData& data, entt::entity& clientEntity)
	{
		// Get the userInput component for this entity
		ComponentView::userInput& usrInput = data.m_reg.get<ComponentView::userInput>(clientEntity);
		
		// Clear the input
		usrInput.clear();

		usrInput.dirty_ = 1;

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
		
		// Only send input if dirty
		if (usrInput.dirty_) {
			std::cout << "Sending control for netid = " << TranslationSystem::getNetId(data, clientEntity) << std::endl;
			NetworkUtilitySystem::sendControl(data, usrInput, TranslationSystem::getNetId(data, clientEntity));
			return true;
		}
		return false;
	}

}

