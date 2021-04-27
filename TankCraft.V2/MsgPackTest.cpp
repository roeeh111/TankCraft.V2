#include "MsgPackTest.h"
#include "ReflectionSystem.h"

namespace msgpackTest {

    // dont know the type:
    // returns the generic object

    // switch by enum, depending on the enum. declare a type, call the unpacking function, then bobs your uncle

    /*
        TODO: in python, have a dev write a schema file, and from that defnine the enum file, and the switch statement!!!!!!!!!!!!!!!!
    */




	void singleStructTest() {

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


        auto res = position();

        // you can convert object to custom class directly
        obj.convert(res);                                                   // NOTE: this fills the allocated container
        // This would be done with either 1) a map of enum compid->component
        //                                2) some sort of templating

        std::cout << res.x << res.y << res.z << std::endl;
        std::cout << obj << std::endl;
	}

    void singleStructStreamTest()
    {
        auto dat = position();
        dat.x = 1;
        dat.y = 2;
        dat.z = 3;
        
      //  int dat = 4;
        msgpack::sbuffer buffer;
        msgpack::packer<msgpack::sbuffer> pk(&buffer);

        auto head = ReflectionSystem::UpdatePacketHeader(69);
        head.ids.push_back(ComponentID::Position);
        head.ids.push_back(ComponentID::MapObject);
        head.ids.push_back(ComponentID::Base);
        pk.pack(head);
        pk.pack(dat);


        msgpack::unpacker pac;

        // feeds the buffer.
        pac.reserve_buffer(buffer.size());
        memcpy(pac.buffer(), buffer.data(), buffer.size());
        pac.buffer_consumed(buffer.size());


        msgpack::object_handle oh;
        bool begin = 1;
        while (pac.next(oh)) {
            if (begin) {
                ReflectionSystem::UpdatePacketHeader header;
                oh.get().convert(header);// GETTING A TYPE ERROR HERE ON UNPACKING!!!!
                std::cout << header.netid << " " << header.ids[0] << std::endl;
            }
            else {
                auto res = position();
                oh.get().convert(res);// GETTING A TYPE ERROR HERE ON UNPACKING!!!!
                 std::cout << res.x << " " << res.y << " " << res.z << std::endl;
            }
            begin = 0;

        }


    //    auto dres = position();
       // int dres;
        //oh.get().convert(dres);// get the first and only data member, and convert it    (ok so we get this problem reguardless)
      //  std::cout << dres.x << " " << dres.y << " " << dres.z << std::endl;
       // std::cout << dres << std::endl;     // OK PERFECT, THE WAY WERE UNPACKING IS WRONG!! NOT THE CLASSES!!! YAAAAAY!!!
    }

    void multiStructStreamTest()
    {
        msgpack::sbuffer buffer;

        auto dat = position();
        dat.x = 1;
        dat.y = 2;
        dat.z = 3;

        auto dat2 = position();
        dat2.x = 4;
        dat2.y = 5;
        dat2.z = 6;

        auto fdat = fltposition();
        fdat.x = 7.0;
        fdat.y = 8.0;
        fdat.z = 9.0;
        auto head = ReflectionSystem::UpdatePacketHeader(69);
        head.ids.push_back(ComponentID::Position);
        head.ids.push_back(ComponentID::MapObject);
        head.ids.push_back(ComponentID::Base);      // TBH it looks like we can get the object just fine...


        // Serialize all components into the buffer
        msgpack::packer<msgpack::sbuffer> pk(&buffer);
        pk.pack(head);
        pk.pack(dat);
        pk.pack(dat2);
        pk.pack(fdat);
        pk.pack(std::string("Josantonio Diaz@"));



        // deserializes these objects using msgpack::unpacker.
        msgpack::unpacker pac;

        // feeds the buffer.
        pac.reserve_buffer(buffer.size());
        memcpy(pac.buffer(), buffer.data(), buffer.size());
        pac.buffer_consumed(buffer.size());

        // now starts streaming deserialization.
        msgpack::object_handle oh;

        pac.next(oh);

        ReflectionSystem::UpdatePacketHeader header;
     //   oh.get().convert(header);// GETTING A TYPE ERROR HERE ON UNPACKING!!!!
      //  pac.next(oh);

        auto dres = position();
        oh.get().convert(dres);// GETTING A TYPE ERROR HERE ON UNPACKING!!!!


        while (pac.next(oh)) {
            std::cout << oh.get() << std::endl;

            // In an actual deserialization function, here we would do the conversions depending on the header values

        }

    }


}


/*
Msgpack would require:

- A header with netid, |compid|compid|compid|
- A body with |component|component|component| (if stream serialization works)
- a map of compid->constructor for a component (to call for obj.convert) (a dev would need to add the compid->constructor mapping somewhere)
*/


/*
For control:
    => exact same method as protobuf, tbh we can keep what protobuf has already

For Login:
    => exact same method as protobuf, tbh we can keep what protobuf has already

For Add Entity/Update Entity:
    => pack in this order: Header, component,component,component
        => use the header to identify what martialing function to use to get the component


NOTE: there are no optional fields embedded into msgpack BUT: we could try and add a bitfield to each component marking what is set and what is not set!!
            -> bitfield would be instantiated similar to the macro with protobuf?
*/


/*
Tasks:

1) make compid->constructor system          (interesting coding challange)
2) fill each networked component with a msgpack defenition (easy)
3) change write/read game update and flush update to be done through msgpack (medium)

lower priority tasks:
4) write python script that generates the switch statement and enum file given a schema
5) change add entity to be done with msgpack
6) set control, remove, login to be with msgpack??


*/
