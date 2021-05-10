#include <iostream>
#include "GameAdmin.h"
#include "ProtoTest.h"
#include "bitseryTest.h"
#include "MsgPackTest.h"
#include "BondTest.h"
#include "MessagingSystem.h"
#include "ReflectionSystem.h"
#include "MsgPackCondDeserialTest.h"


#define MAX_CLIENTS 10

void testLogin();
void startMultiplyerClient();
void testSerialization();
void simpleMsgPackUpdateTest();
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



int main(void)
{
    //msgpackTest::multiStructStreamTest();
    startMultiplyerClient();
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