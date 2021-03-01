#include <iostream>
#include "Serialize.h"
#include "SceneSystem.h"


#define MAX_CLIENTS 10

void playSingleClient();
void playMultiplyerClient();
void testSerialization();

// TODO: fix bitstream/stringstream issues

// TODO: refactor original movement code to control component based

// TODO:   Add Networking through state synchronization

// TODO: add user input system

// TODO??: keep client to server packet at only controls, and server decides what happens to client, and server responds with the new game state
        // -> good for udp, cuz it brings down the amount of interactions from client to server. 



int main(void)
{
    //playSingleClient();
    playMultiplyerClient();
    return 0;
}



void playSingleClient()
{
    // instantiate scene object
    SceneSystem::TanksScene scene = SceneSystem::TanksScene(false, 1);

    // add a single client
    scene.uiSystem.addTank(scene.data, "Roee");

   // scene.printUI();

    // loop and update the game 
    while (true) {
        scene.update();
    }
}

void playMultiplyerClient()
{
    char str[512];
    printf("(C) or (S)erver?\n");
    std::cin >> str;
    bool isServer = (str[0] == 's') || (str[0] == 'S');
    SceneSystem::TanksScene scene = SceneSystem::TanksScene(isServer, 5);
    if (!isServer) scene.uiSystem.addTank(scene.data, "Player 1");

    while (true) {
        scene.update();
    }

}


void testSerialization() {
  //  Tanks::position* p = new Tanks::position();
   // Tanks::position res;
   // p->prevx = 3;

 //   std::stringstream stream = std::stringstream();
  //  Tanks::write<Tanks::position>(stream, *p);
   // Tanks::read<Tanks::position>(stream, res);
}