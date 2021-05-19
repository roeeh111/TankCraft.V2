#pragma once
#include <set>
#include <msgpack.hpp>
#include "IDTranslationComponent.h"
#include "ComponentID.h"


/*
* The header for an update packet. Contains the mapping of all components that are being updated for each netid
* 
*/
namespace UpdatePacketHeader {
	class UpdatePacketHeader {
	public:
		std::set<ComponentID::ComponentID> ids; // The array of component ids
		networkID netid;						   // The netid attached to these components
		MSGPACK_DEFINE(netid, ids);

		UpdatePacketHeader(networkID netid_) {
			ids = std::set<ComponentID::ComponentID>();
			netid = netid_;
		}
		UpdatePacketHeader() {
			ids = std::set<ComponentID::ComponentID>();
			netid = 0;
		}
		~UpdatePacketHeader() = default;
	};
}