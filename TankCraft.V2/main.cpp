#include <iostream>
#include "GameAdmin.h"
#include "ProtoTest.h"
#include "LibNoTest.h"
#include "MessagingSystem.h"


#define MAX_CLIENTS 10

void playMultiplyerClient();
void testSerialization();


int main(void)
{
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