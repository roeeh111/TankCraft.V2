#pragma once

#include <msgpack.hpp>
#include <iostream>

namespace msgpackTest{
	typedef struct position_ {
		uint32_t x;
		uint32_t y;
		uint32_t z;


	public:
		MSGPACK_DEFINE(x, y, z);
		position_() = default;
	} position;

	/*position* getAllocatedPosition() {
		std::cout << "got a position!" << std::endl;
		return new position();
	}
	*/
	typedef struct fltposition_ {
		float x;
		float y;
		float z;

	public:
		MSGPACK_DEFINE(x, y, z);
		fltposition_() = default;
	} fltposition;

	
	/*
namespace msgpack {
	MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS) {
		namespace adaptor {
		}
}
*/




	// Serialize a component into the buffer
//	template <typename T>
//	void Serialize(msgpack::sbuffer &sbuf, T component) {
//		msgpack::pack(component);
//	}

	void singleStructTest();
	void singleStructStreamTest();
	void multiStructStreamTest();





	/*
	template <typename T>
	void templateStructTest() {
		auto dat = position();
		dat.x = 1;
		dat.y = 2;
		dat.z = 3;

		// Serialize into sbuffer
		msgpack::sbuffer sbuf;
		msgpack::pack(sbuf, dat);
		// 

		// Generic Handler for unpacking an object from a buffer
		msgpack::object_handle oh =
			msgpack::unpack(sbuf.data(), sbuf.size());

		// Generic object from handler
		msgpack::object obj = oh.get();

		T res;
		// you can convert object to custom class directly
		obj.convert(res);                                                   // NOTE: this fills the allocated container
		// This would be done with either 1) a map of enum compid->component
		//                                2) some sort of templating

		std::cout << res.x << res.y << res.z << std::endl;
		std::cout << obj << std::endl;
		std::cout << obj.type << std::endl;
	}*/

	// HMM, but we would need to know the type of struct before we call this function......
/*	template <typename T>
	void genericDeserializeTest(msgpack::sbuffer sbuf) {
		// Generic Handler for unpacking an object from a buffer
		msgpack::object_handle oh =
			msgpack::unpack(sbuf.data(), sbuf.size());

		// Generic object from handler
		msgpack::object obj = oh.get();
		obj.type;
		T res;
		// you can convert object to custom class directly
		obj.convert(res);                                                   // NOTE: this fills the allocated container
		// This would be done with either 1) a map of enum compid->component
		//                                2) some sort of templating

		std::cout << obj << std::endl;
		return T;
	}
	*/
}

/*
We could try having each component

*/