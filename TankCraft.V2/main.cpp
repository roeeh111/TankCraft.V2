#include <iostream>
#include "GameAdmin.h"
#include "ProtoTest.h"
#include "bitseryTest.h"
#include "MsgPackTest.h"
#include "BondTest.h"
#include "MessagingSystem.h"
#include "ReflectionSystem.h"
#include "MsgPackCondDeserialTest.h"

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define FRAMES_PER_SECOND	30
#define MAP_WIDTH			20
#define MAP_HEIGHT			20
#define MAX_CLIENTS         10

LRESULT CALLBACK WindowFunc(HWND, UINT, WPARAM, LPARAM);

void testLogin();
void startMultiplyerClient();
void testSerialization();
void simpleMsgPackUpdateTest();

char szWinName[] = "Tank Craft";
char szTitle[] = "Tank Craft v2.1";

bool initialized = false;
bool isServer = false;
int xLoc = 15;
int yLoc = 15;

GameAdmin::MainScene* scene;
UISystem::UISystem* UI;
/*
*   Tasks ordered by priority:
* 
* 1) Get multiplayer working (requires game dump)
* 2) Create script to generate all code required given a schema
* 3) Add more entities to the game, make it an actual game
* 4) Get UI Working
* 5) Clean up code, and delete unused/deprecated code
* 6) Draft up reflection system instructions
* 
*/

int main(HINSTANCE hThisInst, HINSTANCE hPrevInst, LPSTR lpszArgs, int nWinMode) {
	WinMain(hThisInst, hPrevInst, lpszArgs, nWinMode);
	return 0;
}


int WINAPI WinMain(HINSTANCE hThisInst, HINSTANCE hPrevInst, LPSTR lpszArgs, int nWinMode)
{
	HWND hwnd;
	MSG msg;
	WNDCLASSEX wcl;
	UI = new UISystem::UISystem();

	//define the window....
	wcl.cbSize = sizeof(WNDCLASSEX);

	wcl.hInstance = hThisInst;
	wcl.lpszClassName = szWinName;
	wcl.lpfnWndProc = WindowFunc;
	wcl.style = 0;

	wcl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcl.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
	wcl.hCursor = LoadCursor(NULL, IDC_ARROW);

	wcl.lpszMenuName = NULL;
	//wcl.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);

	wcl.cbClsExtra = 0;
	wcl.cbWndExtra = 0;

	//wcl.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
	wcl.hbrBackground = NULL; //if we have a background, BeginPaint will cause flicker

	srand((unsigned)time(NULL));

	//register the window...
	if (!RegisterClassEx(&wcl)) return -1;

	//create the window...
	hwnd = CreateWindowEx(
		NULL,
		szWinName,
		szTitle,
		//WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_THICKFRAME,
		WS_OVERLAPPED | WS_VISIBLE | WS_CAPTION | WS_SYSMENU,
		0, //CW_USEDEFAULT,
		0, //CW_USEDEFAULT,
		640, //CW_USEDEFAULT,
		480, //CW_USEDEFAULT,
		NULL,	//parent window handle
		NULL,	//window menu handle
		hThisInst, //instance handle
		NULL	//creation parms
	);

	//display the window...
	ShowWindow(hwnd, nWinMode);
	UpdateWindow(hwnd);

	//real-time message pump...
	bool quitApplication = false;
	//	CTimer timer(FRAMES_PER_SECOND);

	//	timer.Start();

	while (!quitApplication)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				quitApplication = true;
			}
			else
			{
				if (initialized) scene->update();
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		InvalidateRect(hwnd, NULL, TRUE);
		UpdateWindow(hwnd);
	}
	UnregisterClass(szWinName, wcl.hInstance);
	delete scene;
	return 0;
}

LRESULT CALLBACK WindowFunc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HDC hdcBackBuff;
	static HBITMAP hBitmap;
	static HBITMAP hOldBitmap;
	static HINSTANCE hInstance;

	//ComponentView::userInput& usrInput = scene->data.m_reg.get<ComponentView::userInput>(clientEntity);
	//usrInput.clear();
	//usrInput.dirty_ = 1;

	switch (message)
	{
	case WM_CREATE:
	{
		hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
		hdcBackBuff = CreateCompatibleDC(NULL);
		HDC hdc = GetDC(hwnd);
		hBitmap = CreateCompatibleBitmap(hdc, 640, 480);
		hOldBitmap = (HBITMAP)SelectObject(hdcBackBuff, hBitmap);
		ReleaseDC(hwnd, hdc);
		break;
	}
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_F1:
			initialized = true;
			isServer = true;
			scene = new GameAdmin::MainScene(isServer, MAX_CLIENTS);
			break;
		case VK_F2:
			initialized = true;
			isServer = false;
			scene = new GameAdmin::MainScene(isServer, MAX_CLIENTS);
			UI = new UISystem::UISystem();
			UI->initialize(scene->data);
			break;
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		case VK_RIGHT:
			xLoc++;
			break;
		case VK_LEFT:
			xLoc--;
			break;
		case VK_UP:
			yLoc--;
			break;
		case VK_DOWN:
			yLoc++;
			break;
		}
		break;

	case WM_SIZE:
		break;

	case WM_COMMAND:
		break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		BeginPaint(hwnd, &ps);
		BitBlt(hdcBackBuff, 0, 0, 640, 480, NULL, NULL, NULL, WHITENESS);

		if (!initialized) {
			char buf[256];
			SetTextColor(hdcBackBuff, RGB(0, 0, 255));
			sprintf(&buf[0], "Press F1 to start the server, Press F2 to start the client.");
			TextOut(hdcBackBuff, 125, 50, buf, strlen(buf));
			BitBlt(ps.hdc, 0, 0, 640, 480, hdcBackBuff, 0, 0, SRCCOPY);
			EndPaint(hwnd, &ps);
		} else if (isServer) {
			UI->updateServer(scene->data, hdcBackBuff, ps, hwnd);
		} else {
			UI->updateClient(scene->data, hdcBackBuff, ps, hwnd);
		}

		break;
	}

	case WM_DESTROY:
		SelectObject(hdcBackBuff, hOldBitmap);
		DeleteDC(hdcBackBuff);
		DeleteObject(hBitmap);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}

void startMultiplyerClient() {
    char str;
    printf("(C) or (S)erver?\n");
    std::cin >> str;
    bool isServer = (str == 's') || (str == 'S');
    
    GameAdmin::MainScene scene = GameAdmin::MainScene(isServer, MAX_CLIENTS);

    while (true) {
        scene.update();
    }

}

void simpleMsgPackUpdateTest() {
    // Testing msgpack serialization scheme:
    // make a scene, call all of the init functions
    // add a bunch of components to the map, call flush
    // call MakeGameUpdate on the serialized bunch
    GameAdmin::MainScene scene = GameAdmin::MainScene(true, 1);
    auto pos = new ComponentView::position(1, 0);
    pos->setCurx(6);
    pos->setCury(7);

    auto mp = new ComponentView::MapComponent();
    mp->mp[0] = 60;
    mp->mp[5] = 5;

   // scene.data.updateMap[0] = { pos, new ComponentView::mapObject(), mp};

    ReflectionSystem::UpdatePacket pack(0);
  //  msgpack::sbuffer& stream = pack.Serialize(scene.data.updateMap[0]);

  //  scene.reflectionSystem.DebugDeserialize(scene.data, stream);
    //scene.reflectionSystem.update(scene.data);
    delete pos;
    delete mp;
}

void testLogin() {
    RakNet::BitStream stream = RakNet::BitStream();
    std::string name = "test name";
    MessagingSystem::writeLogin(stream, name);
}

void testSerialization() {
    //ProtoTests::testAddEntity();
    //ProtoTests::testControl();
    //ProtoTests::testUpdateEntity();
    //GameAdmin::TanksScene scene = GameAdmin::TanksScene(false, 1);
    ProtoTests::testGameUpdate();
}