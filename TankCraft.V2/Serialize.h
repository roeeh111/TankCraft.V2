#pragma once

#include "Components.h"
#include <sstream>
#include <msgpack.hpp>
#include "RakNetTypes.h"
#include "BitStream.h"



namespace Serialization {


	// Write the inputted struct to the return paramater stringstream
	// 
	template <typename Type>
	void write(RakNet::BitStream& packed, Type &component) {
		// pack/serialize the stringstream 
		msgpack::pack(packed, component);
	}


	// Read the inputted Serialized structure to an output buffer
	template <typename Type>
	void read(std::stringstream& packed, Type &component) {
		auto const& str = packed.str();
	//	std::string str(reinterpret_cast<char*>(packed.GetData()));
	//	auto const& str = std::string(packed.GetData());

		auto oh = msgpack::unpack(str.data(), str.size());
		auto deserialized = oh.get();

		// Convert it back to the og type
		deserialized.convert(component);

		//std::cout << deserialized << std::endl;
	}
}


// TODO: how do we want to network components??
//		do we write an individual handler for each component?
//		do we write an entity level handler? and it checks every component to see if it should be networked?