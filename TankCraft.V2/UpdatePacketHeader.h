#pragma once
#include <vector>
#include <msgpack.hpp>
#include "IDTranslationComponent.h"
#include "ComponentID.h"

namespace UpdatePacketHeader {
	class UpdatePacketHeader {
	public:
		std::vector<ComponentID::ComponentID> ids; // The array of component ids
		networkID netid;						   // The netid attached to these components
		MSGPACK_DEFINE(netid, ids);

		UpdatePacketHeader(networkID netid_) {
			ids = std::vector<ComponentID::ComponentID>();
			netid = netid_;
		}
		UpdatePacketHeader() {
			ids = std::vector<ComponentID::ComponentID>();
			netid = 0;
		}
		~UpdatePacketHeader() = default;
	};
}