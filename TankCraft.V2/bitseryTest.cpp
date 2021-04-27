#include "bitseryTest.h"

using Buffer = std::vector<uint8_t>;
using OutputAdapter = bitsery::OutputBufferAdapter<Buffer>;
using InputAdapter = bitsery::InputBufferAdapter<Buffer>;


void HelloWorldTest()
{
	MyStruct data{ 8941, "hello", {15.0f, -8.5f, 0.045f} };
	MyStruct res{};

	Buffer buffer;
	auto writtenSize = bitsery::quickSerialization(OutputAdapter{ buffer }, data);
	auto state = bitsery::quickDeserialization(InputAdapter{ buffer.begin(), writtenSize }, res);

	assert(state.first == bitsery::ReaderError::NoError && state.second);
	assert(data.fs == res.fs && data.i == res.i && std::strcmp(data.str, res.str) == 0);
}


