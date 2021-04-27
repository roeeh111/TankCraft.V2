#pragma once

#include <bitsery/bitsery.h>
#include <bitsery/adapter/buffer.h>
#include <bitsery/traits/vector.h>
#include <bitsery/traits/string.h>


struct MyStruct {
	uint32_t i;
	char str[6];
	std::vector<float> fs;
};

template <typename S>
void serialize(S& s, MyStruct& o) {
	s.value4b(o.i);
	s.text1b(o.str);
	s.container4b(o.fs, 100);
}

void HelloWorldTest();