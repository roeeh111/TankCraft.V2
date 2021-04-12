#include <iostream>
#include "GameAdmin.h"
#include "ProtoTest.h"
#include "MessagingSystem.h"


#define MAX_CLIENTS 10

void playMultiplyerClient();
void testSerialization();

// TODO: add component change -> addition of component to update map system (start by hard coding in the movement and scene systems)

// TODO: add control input in networking (add to the update list on change)

// TODO: fix bugs in input system 

// TODO: add replication (kind of done by hardcoding)

//  NOTE: if we settle on protobuf as our replication library, we can change all game components to be protobuf components
//        Right now, keep them seperate so we can keep a layer seperation between protobuf serialization and the rest of our system.
//        In the case that we want to change to another library.

int main(void)
{
   // RakNet::BitStream stream = RakNet::BitStream();
   // std::string name = "sdfasdf";
   // MessagingSystem::writeLogin(stream, name);
    playMultiplyerClient();
    return 0;
}



void playMultiplyerClient()
{
    char str;
    printf("(C) or (S)erver?\n");
    std::cin >> str;
    bool isServer = (str == 's') || (str == 'S');
    GameAdmin::TanksScene scene = GameAdmin::TanksScene(isServer, 5);

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