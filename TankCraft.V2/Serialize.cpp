#include "Serialize.h"
#include "TankObjects.h"



/*
	Implementation of tank serialization and deserialization functions for components
*/

namespace Tanks {
	int donothing(int shit)
	{
		return shit;
	}


	bool read() {
		return 0;
	}

	// LEts see if we aresersads
	template <typename Type>
	bool write(std::stringstream& packed, Type component) {


		// pack/serialize the stringstream 
		/*
		msgpack::pack(packed, component);

		auto const& str = packed.str();
		auto oh = msgpack::unpack(str.data(), str.size());
		auto deserialized = oh.get();

		// Convert it back to the og type
		//deserialized.convert(*pos);


		std::cout << deserialized << std::endl;
		*/
		return 0;
	}


}