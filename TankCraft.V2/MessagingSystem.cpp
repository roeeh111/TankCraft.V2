#include <MessageIdentifiers.h>
#include "MessagingSystem.h"
#include <ctime>

namespace MessagingSystem {
    /*
    * Write defenitions:
    */

    void writeAddEntity(RakNet::BitStream& stream, networkID netid)
    {

        // first, write the packet type to the bitsream
        RakNet::MessageID type = ADD_ENTITY;
        stream.Write((char*)&type, sizeof(RakNet::MessageID));

        // Create a new message object with our fields
        auto msg = new ProtoMessaging::AddRemoveEntityMessage();
        
        msg->set_timestamp(std::time(nullptr));

        // Write the packet to the stream
        stream.Write(msg->SerializeAsString());
    }

    void writeControls(RakNet::BitStream& stream, ComponentView::userInput control)
    {

        // write the packet type to the bitsream
        RakNet::MessageID type = CONTROL;
        stream.Write((char*)&type, sizeof(RakNet::MessageID));

        // Create a new message object with our fields
        auto msg = new ProtoMessaging::ControlMessage();

        //ProtoMessaging::ControlComponent* control = msg->control();
        // RIP PLEASE HELP
        //msg->control().set_up(control.up);
        //msg->control().set_down(control.down);
        msg->set_timestamp(std::time(nullptr));

        // Write the packet to the stream
        stream.Write(msg->SerializeAsString());
    }

    void writeRemoveEntity(RakNet::BitStream& stream, networkID netid)
    {
        // first, write the packet type to the bitsream
        RakNet::MessageID type = REMOVE_ENTITY;
        stream.Write((char*)&type, sizeof(RakNet::MessageID));

        // Create a new message object with our fields
        auto msg = new ProtoMessaging::AddRemoveEntityMessage();
        msg->set_netid(netid);
        msg->set_timestamp(std::time(nullptr));

        // Write the packet to the stream
        stream.Write(msg->SerializeAsString());
    }


    /*
    * Read defenitions:
    */

    ProtoMessaging::AddRemoveEntityMessage* readAddRemoveEntity(std::string stream)
    {
        // Assume we got the first bit of the bitstream, so the bitstream only has our 
        auto msg = new ProtoMessaging::AddRemoveEntityMessage();
        msg->ParseFromString(stream);
        return msg;
    }

    ProtoMessaging::ControlMessage* readControl(std::string stream)
    {
        // Assume we got the first bit of the bitstream, so the bitstream only has our 
        auto msg = new ProtoMessaging::ControlMessage();
        msg->ParseFromString(stream);
        return msg;
    }
}

/*

How do we want to decide what to do when reading.writing entity packets?

I say, use this system to serialize/deserealize the data
And in the network/update layer do the actual decision making

*/

/*
*
* So the general structure is this:
* 
*/
