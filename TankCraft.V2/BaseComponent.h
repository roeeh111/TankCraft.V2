#pragma once
#include "Tanks.pb.h"


typedef struct baseComponent_ {
	//ComponentID type;
	//uint16_t size;		// Limit the size of a component to a uint16, its pretty big, can be changed
	baseComponent_() = default;
	virtual ~baseComponent_() = default;
	virtual void write(ProtoMessaging::UpdateEntityMessage& message, networkID netid) {}
	virtual void read(ProtoMessaging::UpdateEntityMessage& message, networkID netid, int index) {}


} baseComponent;
