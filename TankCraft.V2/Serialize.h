#pragma once

#include "TankObjects.h"
#include <sstream>
#include <msgpack.hpp>




namespace Tanks {


	// Write the inputted struct to the return paramater stringstream
	// 
	template <typename Type>
	void write(std::stringstream& packed, Type component) {
		// pack/serialize the stringstream 
		msgpack::pack(packed, component);
	}


	// Read the inputted Serialized structure to an output buffer
	template <typename Type>
	void read(std::stringstream& packed, Type &component) {
		auto const& str = packed.str();
		auto oh = msgpack::unpack(str.data(), str.size());
		auto deserialized = oh.get();

		// Convert it back to the og type
		deserialized.convert(component);

		std::cout << deserialized << std::endl;
	}
}


// TODO: how do we want to network components??
//		do we write an individual handler for each component?
//		do we write an entity level handler? and it checks every component to see if it should be networked?