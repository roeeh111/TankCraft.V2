#include <MessageIdentifiers.h>
#include "MessagingSystem.h"
#include <ctime>
#include "IDTranslationSystem.h"

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
        delete msg;
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
        delete msg;
    }


    /*
    * Read defenitions:
    */

    ProtoMessaging::AddRemoveEntityMessage* readAddRemoveEntity(std::string &stream)
    {
        // Assume we got the first bit of the bitstream, so the bitstream only has our 
        auto msg = new ProtoMessaging::AddRemoveEntityMessage();
        msg->ParseFromString(stream);
        return msg;
    }

    entt::entity readControls(GameData::GameData& data, std::string& stream, ComponentView::userInput* ret)
    {
        entt::entity ent;

        auto msg = new ProtoMessaging::ControlMessage();
        msg->ParseFromString(stream);

        // if the netid doesnt exist, return nullptr
        if (TranslationSystem::hasMapping(data, msg->control().netid())) {
            ent = TranslationSystem::getEntity(data, msg->control().netid());
        }
        else {
            return entt::null;
        }

        // copy over the values
        ret->down = msg->control().down();
        ret->up = msg->control().up();
        ret->left = msg->control().left();
        ret->right = msg->control().right();


        delete msg;
        return ent;
    }


}