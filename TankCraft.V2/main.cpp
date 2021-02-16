#include <iostream>
#include "TankScene.h"
#include "Serialize.h"

void playSingleClient();

// TODO:   Add Networking through state synchronization

// TODO:   For networking: read and write to a binary stream object??

// TODO??: keep client to server packet at only controls, and server decides what happens to client, and server responds with the new game state
        // -> good for udp, cuz it brings down the amount of interactions from client to server. 



int main(void)
{
    playSingleClient();
}



void playSingleClient()
{
    // instantiate scene object
    Tanks::TanksScene scene = Tanks::TanksScene();
    std::stringstream packed;
    packed << "Number of people is " << 10;
    Tanks::write(packed, 10);

    // add a single client
    scene.addTank("Roee");

    scene.printUI();

    // loop and update the game 
    while (true)
        scene.update();
}