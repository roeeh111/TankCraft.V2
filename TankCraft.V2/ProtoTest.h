#pragma once
#include "Components.h"
#include "Tanks.pb.h"

namespace ProtoTests {
	void testAddEntity();
	void testControl();
	void testUpdateEntity();
	void testGameUpdate(GameData::GameData& data);
	void testGameUpdate();

	void serializationOverheadTest();
	void gameUpdateOverheadTest(int iterations);

	void testOptionalFields();
}
