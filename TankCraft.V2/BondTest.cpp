#include <bond/core/bond.h>
#include <bond/stream/output_buffer.h>
#include "BondTest.h"
#include "example_reflection.h"

namespace BondTest {

    int exampleBufferTest()
    {
        example::Record src;

        src.name = "test";
        src.items.push_back(3.14);

        std::cout << src.name << std::endl;
        bond::OutputBuffer output;
        bond::CompactBinaryWriter<bond::OutputBuffer> writer(output);

        Serialize(src, writer);

        bond::InputBuffer input(output.GetBuffer());
        bond::CompactBinaryReader<bond::InputBuffer> reader(input);

        example::Record dst;

        Deserialize(reader, dst);

        return 0;
    }
}
