#pragma once


// An enumeration for the component type
// Currently used by the reflection system
namespace ComponentID {
	typedef enum {
		Base,
		Position,
		MapObject,
		Score,
		ClientName,
		DirtyClient,
		Health,
		DamageDone,
		input
	} ComponentID;
};
