#include <iostream>
#include "TankScene.h"
#include "Serialize.h"

void playSingleClient();

// TODO:   Implement generic serialization through a read and write function

// TODO:   Add Networking through state synchronization

// TODO:   For networking: read and write to a binary stream object

// TODO??: keep client to server packet at only controls, and server decides what happens to client, and server responds with the new game state
        // -> good for udp, cuz it brings down the amount of interactions from client to server. 



int main(void)
{
    std::stringstream packed;
    Tanks::position* pos = new Tanks::position();

    pos->prevx = 1;
    pos->prevy = 2;
    pos->curx = 3;
    pos->cury = 4;
     Tanks::write<Tanks::position>(packed, *pos);
    //Serialization::write<std::string>(packed, "sand");

  //  Serialization::donothing(2);

  //  Serialization::donothing(1); // This works normally




}



void playSingleClient()
{
    // instantiate scene object
    Tanks::TanksScene scene = Tanks::TanksScene();

    // add a single client
    scene.addTank("Roee");

    scene.printUI();

    // loop and update the game 
    while (true)
        scene.update();
}