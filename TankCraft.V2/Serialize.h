#pragma once

#include "Components.h"
#include <iostream>
#include <msgpack.hpp>
#include "RakNetTypes.h"
#include "BitStream.h"
#include <sstream>


// TODO: this implementation is slow, it creates a stringstream from a bitstream in order to serialize.
//		 Go back and optimize this process. Its this way now because i want to move on to other things.



namespace Serialization {


	// Write the inputted struct to the return paramater stringstream
	// 
	template <typename Type>
	void write(RakNet::BitStream& packed, Type &component) {
		std::stringstream cheating = std::stringstream();
		
		unsigned char* c = packed.GetData();
//		std::string s = std::string(c);
		// copy over the values from the bitstream to the stringstream
		std::stringstream str = std::stringstream();
		str << c;
		std::cout << str.str() << std::endl;


		// pack/serialize the stringstream 
		msgpack::pack(str, component);
	}

	// Read the inputted Serialized structure to an output buffer
	template <typename Type>
	void read(RakNet::BitStream& packed, Type& component) {

		// I could also ask for a stringstream and convert it to a bitstream, but that seems slow to me
		// lets just start with doing that for now...


		// get a string version of the bitstream
	//	packed.Read(str, packed.GetData());
		//auto const& str = packed.str();

		// read into the string
	//	while (packed.Read(buf, 50));
	//	std::string str = std::string(buf);


		std::cout << "getting string version of bitstream" << std::endl;

		std::string str = std::string(reinterpret_cast<char*>(packed.GetData()));
		std::cout << str << std::endl;
		std::cout << str.size() << std::endl;
		std::cout << "unpacking to handler" << std::endl;

		auto oh = msgpack::unpack(str.data(), str.size());

		std::cout << "unpacking" << std::endl;


		auto deserialized = oh.get();

		std::cout << "converting back to type" << std::endl;

		// Convert it back to the og type
		deserialized.convert(component);

		std::cout << deserialized << std::endl;
	}




	template <typename Type>
	void write(std::stringstream& packed, Type& component) {
		// pack/serialize the stringstream 
		msgpack::pack(packed, component);
	}

	template <typename Type>
	void read(std::stringstream& packed, Type& component) {

		// get a string version of the bitstream
	//	std::string str();
	//	packed.Read(str, packed.GetData());
		//auto const& str = packed.str();

		// read into the string
		//while (packed.Read(reinterpret_cast<char*>(str), 50));

		std::string  str = packed.str();
		std::cout << "getting string version of bitstream" << std::endl;

	//	std::string str(reinterpret_cast<char*>(packed.GetData()));
		std::cout << str << std::endl;
		std::cout << str.size() << std::endl;
		std::cout << "unpacking to handler" << std::endl;

		auto oh = msgpack::unpack(str.data(), str.size());

		std::cout << "unpacking" << std::endl;


		auto deserialized = oh.get();

		std::cout << "converting back to type" << std::endl;

		// Convert it back to the og type
		deserialized.convert(component);

		std::cout << deserialized << std::endl;
	}

	
}

