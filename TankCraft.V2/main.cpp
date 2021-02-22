#include <iostream>
#include "TankScene.h"
#include "Components.h"
#include "Serialize.h"

void playSingleClient();
void testSerialization();

// TODO: fix bitstream/stringstream issues

// TODO: refactor original movement code to control component based

// TODO:   Add Networking through state synchronization

// TODO: add user input system

// TODO??: keep client to server packet at only controls, and server decides what happens to client, and server responds with the new game state
        // -> good for udp, cuz it brings down the amount of interactions from client to server. 



int main(void)
{
    playSingleClient();

    return 0;
}



void playSingleClient()
{
    // instantiate scene object
    GameView::TanksScene scene = GameView::TanksScene(false, 1);

    // add a single client
    scene.addClient("Roee");

    scene.printUI();

    // loop and update the game 
    while (true)
        scene.update();
}

void testSerialization() {
  //  Tanks::position* p = new Tanks::position();
   // Tanks::position res;
   // p->prevx = 3;

 //   std::stringstream stream = std::stringstream();
  //  Tanks::write<Tanks::position>(stream, *p);
   // Tanks::read<Tanks::position>(stream, res);
}