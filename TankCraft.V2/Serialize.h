#pragma once

#include "TankObjects.h"
#include <sstream>
#include <msgpack.hpp>




namespace Tanks {


	// Write the inputted struct to an output buffer
	template <typename Type>
	bool write(std::stringstream& packed, Type component);


	int donothing(int shit);



	// Read the inputted Serialized structure to an output buffer
	bool read(); // TODO: add required fields

}


// TODO: how do we want to network components??
//		do we write an individual handler for each component?
//		do we write an entity level handler? and it checks every component to see if it should be networked?