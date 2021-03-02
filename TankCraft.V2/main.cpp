#include <iostream>
#include "Serialize.h"
#include "SceneSystem.h"


#define MAX_CLIENTS 10

void playSingleClient();
void playMultiplyerClient();
void testSerialization();
void testPacket();

// TODO: fix bitstream/stringstream issues

// TODO: refactor original movement code to control component based (fix the bugs)

// TODO: Add Networking through state synchronization

// TODO: add user input system (fix the bugs) 


int main(void)
{
    //playSingleClient();
    playMultiplyerClient();
    return 0;
}

// updates field is correct
void testPacket() {
    networkID s = 5;
    auto stream = std::stringstream();
    Packets::UpdatePacket pack = Packets::UpdatePacket(s);
    ComponentView::userInput in = ComponentView::userInput();
    in.up = 1;
    in.down = 1;
    in.left = 0;
    in.right = 0;

    pack.write(stream);

    auto res = std::stringstream();
    pack.read(res);

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
  /*
    ComponentView::position* p = new ComponentView::position();
    ComponentView::position res;
    ComponentView::position res2;
    p->prevx = 3;
    p->cury = 6;

    std::stringstream ss = std::stringstream();
    Serialization::write<ComponentView::position>(ss, *p);      // Writing has no problems....
    Serialization::read<ComponentView::position>(ss, res2);

 //   std::strin stream = std::stringstream();
    RakNet::BitStream stream = RakNet::BitStream();
    Serialization::write<ComponentView::position>(stream, *p);      // Writing has no problems....
    Serialization::read<ComponentView::position>(stream, res);

    assert(p->cury == res.cury);
    assert(p->prevx == res.prevx);
    std::cout << "finito" << std::endl;
    */
}