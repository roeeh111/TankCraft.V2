#pragma once
#include "TankObjects.h"

namespace NetworkSystem {
	class Packet {
	public:
		Packet() { type = gameUpdate; }
	private:
		enum Packet_Type type;
	};


	enum Packet_Type {
		gameUpdate,
		addEntity,
		removeEntity,
		controlInput
	};

}
