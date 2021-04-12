#include <MessageIdentifiers.h>
#include "MessagingSystem.h"
#include <ctime>
#include "IDTranslationSystem.h"

namespace MessagingSystem {

    // System flush
    void FlushGameUpdate(GameData::GameData& data) {
        if (data.updateMap.size() <= 0)
            return;
        std::cout << "Flushing game update of size " << data.updateMap.size() << std::endl;
        RakNet::BitStream stream = RakNet::BitStream();
        MessagingSystem::writeGameUpdate(stream, data.updateMap);  

        // Broadcast the game update
        data.rpi->Send(&stream,
            HIGH_PRIORITY,
            RELIABLE_ORDERED,
            0,
            RakNet::UNASSIGNED_SYSTEM_ADDRESS,
            true);
    }


    /*
    * Write defenitions:
    */

    void writeAddEntity(RakNet::BitStream& stream, networkID netid)
    {

        // first, write the packet type to the bitsream
        RakNet::MessageID type = ADD_ENTITY;
        stream.Write((char*)&type, sizeof(RakNet::MessageID));

        // Create a new message object with our fields
        auto msg = ProtoMessaging::AddRemoveEntityMessage();
        
        msg.set_timestamp(std::time(nullptr));

        // Write the packet to the stream
        stream.Write(msg.SerializeAsString());
    }

    void writeControls(RakNet::BitStream& stream, ComponentView::userInput control, networkID netid)
    {

        // write the packet type to the bitsream
        RakNet::MessageID type = CONTROL;
        stream.Write((char*)&type, sizeof(RakNet::MessageID));

        // Create a new message object with our fields
        auto msg = ProtoMessaging::ControlMessage();

        // The control component, fill its values
        auto controlComp = new ProtoMessaging::ControlComponent();
        controlComp->set_up(control.up());
        controlComp->set_down(control.down());
        controlComp->set_left(control.left());
        controlComp->set_right(control.right());
        controlComp->set_netid(netid);

        msg.set_allocated_control(controlComp);
        msg.set_timestamp(std::time(nullptr));

        // Write the packet to the stream
        stream.Write(msg.SerializeAsString());

        delete controlComp;
    }
    
    // Flush the update map
    void writeGameUpdate(RakNet::BitStream& stream, std::map<networkID, std::list<baseComponent*>>& updateMap)
    {
        // write the packet type to the bitsream
        RakNet::MessageID type = UPDATE_ENTITY;
        stream.Write((char*)&type, sizeof(RakNet::MessageID));

        // our message
        auto message = ProtoMessaging::UpdateEntityMessage();


        // We only have 1 key in the update map, which is ok
        // Iterate through the map, writing to the message          
        for (auto& myPair : updateMap) {                            // PROBLEM: getting a read access violation when trying to iterate to the next element
            std::cout << "Writegameupdate, found an element. Has "<< myPair.second.size() << " element id = " << myPair.first << std::endl;
            // If the pair is an actual game update
            for (auto& component: myPair.second) {
                std::cout << "  Writegameupdate, writing a component " << component->CompId << std::endl;
                component->write(message, myPair.first);

            }
        }
        updateMap.clear();

        // Set the timestamp
        message.set_timestamp(std::time(nullptr));

        // Serialize and write the packet to the stream
        stream.Write(message.SerializeAsString());
    }

    void writeRemoveEntity(RakNet::BitStream& stream, networkID netid)
    {
        // first, write the packet type to the bitsream
        RakNet::MessageID type = REMOVE_ENTITY;
        stream.Write((char*)&type, sizeof(RakNet::MessageID));

        // Create a new message object with our fields
        auto msg =  ProtoMessaging::AddRemoveEntityMessage();
        msg.set_netid(netid);
        msg.set_timestamp(std::time(nullptr));

        // Write the packet to the stream
        stream.Write(msg.SerializeAsString());
    }

    // DEBUG ONLY (doesnt write to bitstream)
    std::string writeGameUpdate(std::map<networkID, std::list<baseComponent*>>& updateMap)
    {
        // our message
        auto message = ProtoMessaging::UpdateEntityMessage();

        // Iterate through the map, writing to the message 
        for (auto& myPair : updateMap) {
            for (auto& component : myPair.second) {
                component->write(message, myPair.first);

            }
            updateMap[myPair.first].clear();
        }

        // Set the timestamp
        message.set_timestamp(std::time(nullptr));

        // Serialize and write the packet to the stream
        return message.SerializeAsString();
    }

    void writeLogin(RakNet::BitStream& stream, std::string& name)
    {
        // first, write the packet type to the bitsream
        RakNet::MessageID type = LOGIN;
        stream.Write((char*)&type, sizeof(RakNet::MessageID));

        auto temp1 = (char*)stream.GetData();
       // std::cout << temp1 << strlen(temp1) << std::endl;

        // Create a new message object with our fields
        auto msg = ProtoMessaging::LoginMessage();
        msg.set_name(name);
        std::cout << "in writelogin, name = " << msg.name() << std::endl;
       
        // Write the packet to the stream
       // auto serializedMsg = new std::string;
        auto serializedMsg = msg.SerializeAsString();
        stream.Write(serializedMsg.c_str(), serializedMsg.size());

        // Debug:
        // get a string representation of the string, skipping over the login enum

        // deserialize and print what you get
   //     auto temp = (char *) stream.GetData();
    //    std::cout << temp << strlen(temp) << std::endl;
     //   if ((unsigned char) temp[0] == LOGIN) {
     //       std::cout << "Login packet type is correct" << std::endl;
      //  }
      //  else {
      //      std::cout << "Login packet type is incorrect" << std::endl;
      //  }
      //  std::string stream2 = std::string((char*)(temp + 1));
       // std::string loginName = MessagingSystem::readLogin(stream2);
        //std::cout << "in writelogin, Adding tank for " << loginName << std::endl;
    }

    /*
    * Read defenitions:
    */


    void readGameUpdate(GameData::GameData& data, std::string& stream)
    {
        // Parse the message from our string
        auto msg = ProtoMessaging::UpdateEntityMessage();
        msg.ParseFromString(stream);

        // TODO: do some verification (for udp)

        // For all fields in the packet, loop over and call readComp
        auto desc = msg.GetDescriptor();
        auto refl = msg.GetReflection();
        int fieldCount = desc->field_count();

        // loop over all of the fields, geting their descriptor
        for (int i = FIRST_COMPONENT_IN_UPDATE; i < fieldCount; i++)
        {
            // get the field from the descriptor
            auto field = desc->field(i);
            auto size = refl->FieldSize(msg, field);
            
            // FOR NOW, HARD CODE WHATS GETTING UPDATED, AND GENERICIZE LATER (bad looking code, but will be phased out later)
        //    refl->GetRepeatedFieldRef(msg, field);
            // case statement of calling x function for x field
            switch (i) { 
                case FIRST_COMPONENT_IN_UPDATE: {
                    // updating positions, call readcomp for all positions
                    // for all repetitions in this field, call readcomps and cast this message
                    for (int j = 0; j < msg.positioncomps_size(); j++) {
                        readPosComp(data, msg, j);
                    }
                }
                case (FIRST_COMPONENT_IN_UPDATE + 1): {
                    for (int j = 0; j < msg.mapobjectcomps_size(); j++) {
                        readObjComp(data, msg, j);
                    }
                }
                case (FIRST_COMPONENT_IN_UPDATE + 2): {
                    //for (int j = 0; j < msg.scorecomps_size(); j++) {
                    //    readComp(data, msg.scorecomps().Get(j));
                    //}
                }
                case (FIRST_COMPONENT_IN_UPDATE + 3) : {
                    for (int j = 0; j < msg.clientnamecomps_size(); j++) {
                        readNameComp(data, msg, j);
                    }
                }
                case (FIRST_COMPONENT_IN_UPDATE + 4): {
                 //   for (int j = 0; j < msg.healthcomps_size(); j++) {
                 //       readComp(data, msg.healthcomps().Get(j));
                 //   }
                }
            }



            // For all valid repetitions of this field
         //   for (int j = 0; j < size; j++) {
                // get the message that is repeated
          //      refl->GetRepeatedMessage(msg, field, j).GetReflection();
          //  }
            // TODO: update the actual component
        }
    }

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
        ret->setDown(msg->control().down());
        ret->setUp(msg->control().up());
        ret->setLeft(msg->control().left());
        ret->setRight(msg->control().right());


        delete msg;
        return ent;
    }

    std::string readLogin(std::string& stream)
    {
        auto msg = new ProtoMessaging::LoginMessage();
        msg->ParseFromString(stream);
        std::cout << "in read login: name = " << msg->name() << std::endl;
        msg->PrintDebugString();
        std::string res(msg->name());
        delete msg;
        return res;
    }


    /*
    *  Protobuf component to Game component functions
    */

    
    void readPosComp(GameData::GameData& data, ProtoMessaging::UpdateEntityMessage &msg, int index)
    {
        std::cout << "Reading position component" << std::endl;

        auto comp = msg.positioncomps().Get(index);

        // get the entity in the register, and swap over the values
       // ComponentView::position &pos = data.m_reg.get<ComponentView::position>(TranslationSystem::getEntity(data, comp.netid()));
        
        auto ref = comp.GetReflection();

        const google::protobuf::Descriptor* positiondescriptor = comp.GetDescriptor();
        const google::protobuf::FieldDescriptor* prevx_field = positiondescriptor->FindFieldByName("prevx");
        const google::protobuf::FieldDescriptor* prevy_field = positiondescriptor->FindFieldByName("prevy");
        const google::protobuf::FieldDescriptor* curx_field = positiondescriptor->FindFieldByName("curx");
        const google::protobuf::FieldDescriptor* cury_field = positiondescriptor->FindFieldByName("cury");

        // TODO: what if i dont set some value?

        //  pos.prevx = comp.prevx();
        std::cout << "prevx = " << comp.prevx() << std::endl;
        // pos.prevy = comp.prevy();
        std::cout << "prevy = " << comp.prevy() << std::endl;
        // pos.curx = comp.curx();
        std::cout << "curx = " << comp.curx() << std::endl;
        // pos.cury = comp.cury();
        std::cout << "cury = " << comp.cury() << std::endl;
    }

    void readObjComp(GameData::GameData& data, ProtoMessaging::UpdateEntityMessage& msg, int index)
    {
        auto comp = msg.mapobjectcomps().Get(index);
        // get the entity in the register, and swap over the values
        ComponentView::mapObject &obj = data.m_reg.get<ComponentView::mapObject>(TranslationSystem::getEntity(data, comp.netid()));

        auto ref = comp.GetReflection();
        const google::protobuf::Descriptor* objectdescriptor = comp.GetDescriptor();
        const google::protobuf::FieldDescriptor* mapchar_field = objectdescriptor->FindFieldByName("mapChar");

        obj.setMapChar(comp.mapchar()[0]);
    }

    void readNameComp(GameData::GameData& data, ProtoMessaging::UpdateEntityMessage& msg, int index)
    {
        auto comp = msg.clientnamecomps().Get(index);

        // get the entity in the register, and swap over the values
        ComponentView::clientName& name = data.m_reg.get<ComponentView::clientName>(TranslationSystem::getEntity(data, comp.netid()));
        auto ref = comp.GetReflection();
        const google::protobuf::Descriptor* namedescriptor = comp.GetDescriptor();
        const google::protobuf::FieldDescriptor* name_field = namedescriptor->FindFieldByName("name");

        name.setName(comp.name());
    }

}