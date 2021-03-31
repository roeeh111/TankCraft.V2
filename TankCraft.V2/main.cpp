#include <iostream>
#include "GameAdmin.h"
#include "ProtoTest.h"


#define MAX_CLIENTS 10

void playSingleClient();
void playMultiplyerClient();
void testSerialization();

// (add addition to update queue, and flush of update queue)

// TODO: add login process, where client sends over to add an entity, then updates that entity with its username

// TODO: add component change -> addition of component to update map system (start by hard coding in the movement and scene systems)

// TODO: add initialization of netid->component list in data map somewhere in the code (probably on entity creation)

// TODO: add control input in networking (add to the update list on change)

// TODO: in Id translation system, set that only the server creates a freelist. (need to change all the get,set methods so that a client doesnt access the freelist)

// TODO: fix bugs in input system 

// TODO: add replication (kind of done by hardcoding)

//  NOTE: if we settle on protobuf as our replication library, we can change all game components to be protobuf components
//        Right now, keep them seperate so we can keep a layer seperation between protobuf serialization and the rest of our system.
//        In the case that we want to change to another library.

int main(void)
{
    //playSingleClient();
    //playMultiplyerClient();
    testSerialization();
    return 0;
}


void playSingleClient()
{
    // instantiate scene object
    GameAdmin::TanksScene scene = GameAdmin::TanksScene(false, 1);

    // add a single client
    UI::addTank(scene.data, "Roee");

   // scene.printUI();

    // loop and update the game 
    while (true) {
        scene.update();
    }
}

void playMultiplyerClient()
{
    char str;
    printf("(C) or (S)erver?\n");
    std::cin >> str;
    bool isServer = (str == 's') || (str == 'S');
    GameAdmin::TanksScene scene = GameAdmin::TanksScene(isServer, 5);
    //if (!isServer) scene.uiSystem.addTank(scene.data, "Player 1");

    while (true) {
        scene.update();
    }

}


void testSerialization() {
    //ProtoTests::testAddEntity();
    //ProtoTests::testControl();
    //ProtoTests::testUpdateEntity();
    //GameAdmin::TanksScene scene = GameAdmin::TanksScene(false, 1);

    ProtoTests::testGameUpdate();
}