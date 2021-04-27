#include <iostream>
#include "GameAdmin.h"
#include "ProtoTest.h"
#include "bitseryTest.h"
#include "MsgPackTest.h"
#include "MessagingSystem.h"
#include "ReflectionSystem.h"

#define MAX_CLIENTS 10

void testLogin();
void startMultiplyerClient();
void testSerialization();

/* TODO List:
    1. add component change -> addition of component to update map system (start by hard coding in the movement and scene systems)

    2. add control input in networking (add to the update list on change)

    3. fix bugs in input system

    4. add replication (kind of done by hardcoding)
*/

//  NOTE: if we settle on protobuf as our replication library, we can change all game components to be protobuf components
//        Right now, keep them seperate so we can keep a layer seperation between protobuf serialization and the rest of our system.
//        In the case that we want to change to another library.
int main(void)
{
    // Testing msgpack serialization scheme:
    // make a scene, call all of the init functions
    // add a bunch of components to the map, call flush
    // call MakeGameUpdate on the serialized bunch
    GameAdmin::MainScene scene = GameAdmin::MainScene(true, 1);
    auto pos = new ComponentView::position(1, 0);
    pos->setCurx(6);
    pos->setCury(7);
    scene.data.updateMap[0] = {pos, new ComponentView::mapObject()};

    ReflectionSystem::UpdatePacket pack(0);
    std::cout << "Calling serialize" << std::endl;
    msgpack::sbuffer &stream = pack.Serialize(scene.data.updateMap[0]);
    std::cout << "serialized the message, now calling game update" << std::endl;

    ReflectionSystem::MakeGameUpdate(scene.data, stream);           // Getting error on this line, when trying to deserialize the packet header
    delete pos;
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

void testLogin() {
    MessagingSystem::MessagingSystem messagingSystem;
    RakNet::BitStream stream = RakNet::BitStream();
    std::string name = "test name";
    messagingSystem.writeLogin(stream, name);
}

void testSerialization() {
    //ProtoTests::testAddEntity();
    //ProtoTests::testControl();
    //ProtoTests::testUpdateEntity();
    //GameAdmin::TanksScene scene = GameAdmin::TanksScene(false, 1);
    ProtoTests::testGameUpdate();
}