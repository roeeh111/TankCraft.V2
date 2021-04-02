#pragma once
#include "Tanks.pb.h"


typedef struct baseComponent_ {
	//ComponentID type;
	baseComponent_() { dirty = false; }
	virtual ~baseComponent_() = default;
	virtual void write(ProtoMessaging::UpdateEntityMessage& message, networkID netid) { std::cout << "base write " << std::endl; }
	bool getDirty() { return dirty; }
	//virtual void read(ProtoMessaging::UpdateEntityMessage& message, networkID netid, int index) {}
	bool dirty;
} baseComponent;
