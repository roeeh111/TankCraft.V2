#include <MessageIdentifiers.h>
#include "MessagingSystem.h"
#include <ctime>
#include "IDTranslationSystem.h"

namespace MessagingSystem {

    void MessagingSystem::init(GameData::GameData& data) {

    }
    // SERVER ONLY: System flush
    void MessagingSystem::update(GameData::GameData& data) {
        if (!data.isServer) return;
    //    if (data.updateMap.size() <= 0)
            return;
     //   std::cout << "Flushing game update of size " << data.updateMap.size() << std::endl;
      ///  std::cout << "Comp list size: " << data.updateMap[0].size() << std::endl;
      //  RakNet::BitStream stream = RakNet::BitStream();
      //  writeGameUpdate(stream, data.updateMap);

        // Broadcast the game update     
     //  data.rpi->Send(&stream,
      //      HIGH_PRIORITY,
      //      RELIABLE_ORDERED,
      //      0,
      //      RakNet::UNASSIGNED_SYSTEM_ADDRESS,
       //     true);
      //  NetworkUtilitySystem::broadcast(data, &stream, HIGH_PRIORITY, RELIABLE_ORDERED, 0);
    }

    /*
    * Write definitions:
    */
    void writeAddEntity(RakNet::BitStream& stream, networkID netid)
    {

        // first, write the packet type to the bitsream
        RakNet::MessageID type = ADD_ENTITY;
        stream.Write((char*)&type, sizeof(RakNet::MessageID));

        // Create a new message object with our fields
        auto msg = ProtoMessaging::AddRemoveEntityMessage();
        msg.set_netid(netid);// = netid;                        // NEW, added this line, because it doesnt seem like were actually adding a netid
        //std::cout << "Called writeAddEntity, adding entity with netid = " << msg.netid() << std::endl;

        msg.set_timestamp(std::time(nullptr));
      //  std::cout << "Timestamp = " << msg.timestamp() << std::endl;

        // Write the packet to the stream
      //  stream.Write(msg.SerializeAsString());
        auto serializedMsg = msg.SerializeAsString();
        stream.Write(serializedMsg.c_str(), serializedMsg.size());

    }

    // TODO: I THINK THIS IS WRONG AND THATS WHY WERE ONLY GETTING ZEROS
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

       // std::cout << "In writecontrols: left = " << controlComp->left() << " right = " << controlComp->right() << " up = " << controlComp->up() << " down = " << controlComp->down() << std::endl;


        // Write the packet to the stream
       // stream.Write(msg.SerializeAsString());
        auto serializedMsg = msg.SerializeAsString();
        stream.Write(serializedMsg.c_str(), serializedMsg.size());

        //  delete controlComp; // WE BREAK ON THIS LINE. SO MAYBE DONT DO THIS?? 
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
            // std::cout << "Writegameupdate, found an entity with "<< myPair.second.size() << " components to update. EntityID = " << myPair.first << std::endl;
            // If the pair is an actual game update
            for (auto& component : myPair.second) {
                //std::cout << "  Writegameupdate, writing a component " << component->CompId << std::endl;
                component->write(message, myPair.first);

            }
        }
        updateMap.clear();

        // Set the timestamp
        message.set_timestamp(std::time(nullptr));


        // Serialize and write the packet to the stream
        auto serializedMsg = message.SerializeAsString();
        stream.Write(serializedMsg.c_str(), serializedMsg.size());

        ///////////////////////////////////////////////// DEBUG /////////////////////////////////////////////////
        auto deserializedMsg = ProtoMessaging::UpdateEntityMessage();
        deserializedMsg.ParseFromString(serializedMsg);

      //  std::cout << "time stamp: " << deserializedMsg.timestamp() << std::endl;
        auto desc = deserializedMsg.GetDescriptor();
        auto refl = deserializedMsg.GetReflection();
        int fieldCount = desc->field_count();
     //   std::cout << "id: " << desc->field(0) << std::endl;
        // std::cout << "  Found " << fieldCount << " Fields" << std::endl;
             // loop over all of the fields, geting their descriptor
        for (int i = FIRST_COMPONENT_IN_UPDATE; i < fieldCount; i++)
        {
            // get the field from the descriptor
            auto field = desc->field(i);
            auto size = refl->FieldSize(message, field);
         //   std::cout << "field = " << i << " name = " << field->name() << " size = " << size << std::endl;

        }
        auto posComp = deserializedMsg.positioncomps().Get(0);
        std::cout << "Before send: curX = " << posComp.curx() << " curY = " << posComp.cury() << std::endl;
        auto nameComp = deserializedMsg.clientnamecomps().Get(0);
        std::cout << "Before send: name = " << nameComp.name() << std::endl;

        ///////////////////////////////////////////////// DEBUG /////////////////////////////////////////////////
    }

    void writeRemoveEntity(RakNet::BitStream& stream, networkID netid)
    {
        // first, write the packet type to the bitsream
        RakNet::MessageID type = REMOVE_ENTITY;
        stream.Write((char*)&type, sizeof(RakNet::MessageID));

        // Create a new message object with our fields
        auto msg = ProtoMessaging::AddRemoveEntityMessage();
        msg.set_netid(netid);
        msg.set_timestamp(std::time(nullptr));

        // Write the packet to the stream
        auto serializedMsg = msg.SerializeAsString();
        stream.Write(serializedMsg.c_str(), serializedMsg.size());
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


        // Create a new message object with our fields
        auto msg = ProtoMessaging::LoginMessage();
        msg.set_name(name);
        std::cout << "in writelogin, name = " << msg.name() << std::endl;

        // Write the packet to the stream
       // auto serializedMsg = new std::string;
        auto serializedMsg = msg.SerializeAsString();
        stream.Write(serializedMsg.c_str(), serializedMsg.size());
    }

    /*
    * Read definitions:
    */
    // TODO: PROBLEM: NOT CALLING THE READCOMP FUNCTIONS (because it shows that we have 0 elements in those fields)
    // 1) Look at debug string on flush call. 2) call readgameupdate inside flush call
    void readGameUpdate(GameData::GameData& data, std::string& stream)
    {
        //    std::cout << "In read game update!" << std::endl;
            // Parse the message from our string
        auto msg = ProtoMessaging::UpdateEntityMessage();
        msg.ParseFromString(stream);
        
        // TODO: do some verification (for udp)

        // For all fields in the packet, loop over and call readComp
        auto desc = msg.GetDescriptor();
        auto refl = msg.GetReflection();
        int fieldCount = desc->field_count();
        // std::cout << "  Found " << fieldCount << " Fields" << std::endl;
        std::cout << "time stamp: " << msg.timestamp() << std::endl;
        std::cout << "id: " << desc->field(0) << std::endl;
        // loop over all of the fields, geting their descriptor
        for (int i = FIRST_COMPONENT_IN_UPDATE; i < fieldCount; i++)
        {
            // get the field from the descriptor
            auto field = desc->field(i);
            auto size = refl->FieldSize(msg, field);
            std::cout << "field = " << i << " name = " << field->name() << " size = " << size << std::endl;

            // FOR NOW, HARD CODE WHATS GETTING UPDATED, AND GENERICIZE LATER (bad looking code, but will be phased out later)
        //    refl->GetRepeatedFieldRef(msg, field);
            // case statement of calling x function for x field
            switch (i) {
            case FIRST_COMPONENT_IN_UPDATE: {
                //        std::cout << "in position update case number of updates = " << msg.positioncomps_size() << std::endl;
                        // updating positions, call readcomp for all positions
                        // for all repetitions in this field, call readcomps and cast this message
                for (int j = 0; j < msg.positioncomps_size(); j++) {
                    readPosComp(data, msg, j);
                }
                break;
            }
            case (FIRST_COMPONENT_IN_UPDATE + 1): {
                //        std::cout << "in mapobj update case number of updates = " << msg.mapobjectcomps_size() << std::endl;
                for (int j = 0; j < msg.mapobjectcomps_size(); j++) {
                    readObjComp(data, msg, j);
                }
                break;
            }
            case (FIRST_COMPONENT_IN_UPDATE + 2): {
                //for (int j = 0; j < msg.scorecomps_size(); j++) {
                //    readComp(data, msg.scorecomps().Get(j));
                //}
                break;
            }
            case (FIRST_COMPONENT_IN_UPDATE + 3): {
                //         std::cout << "in clientname update case number of updates = " << msg.clientnamecomps_size() << std::endl;
                for (int j = 0; j < msg.clientnamecomps_size(); j++) {
                    readNameComp(data, msg, j);
                }
                break;
            }
            case (FIRST_COMPONENT_IN_UPDATE + 4): {
                //   for (int j = 0; j < msg.healthcomps_size(); j++) {
                //       readComp(data, msg.healthcomps().Get(j));
                //   }
                break;
            }

            case (FIRST_COMPONENT_IN_UPDATE + 5): {
                //         std::cout << "in userinput update case number of updates = " << msg.uinputcomps_size() << std::endl;
                for (int j = 0; j < msg.uinputcomps_size(); j++) {
                    readControlComp(data, msg, j);
                }
                break;
            }

            default: {
                std::cout << "DEFAULT case, do nothing" << std::endl;
                break;
            }
            }

            // For all valid repetitions of this field
         //   for (int j = 0; j < size; j++) {
                // get the message that is repeated
          //      refl->GetRepeatedMessage(msg, field, j).GetReflection();
          //  }
            // TODO: update the actual component
        }
        //   std::cout << "Exiting read update" << std::endl;

    }

    ProtoMessaging::AddRemoveEntityMessage* readAddRemoveEntity(std::string& stream)
    {
        // Assume we got the first bit of the bitstream, so the bitstream only has our 
        auto msg = new ProtoMessaging::AddRemoveEntityMessage();
        msg->ParseFromString(stream);
        return msg;
    }

    ComponentView::userInput& readControls(GameData::GameData& data, std::string& stream, entt::entity * ent)
    {
        auto msg = new ProtoMessaging::ControlMessage();
        msg->ParseFromString(stream);

        // if the netid doesnt exist, return nullptr
        if (TranslationSystem::hasMapping(data, msg->control().netid())) {
            *ent = TranslationSystem::getEntity(data, msg->control().netid());
        }


        std::cout << "Handling control for netid: " << msg->control().netid() << " With entt id: " << (int) *ent << std::endl;
        auto &ret = data.m_reg.get<ComponentView::userInput>(*ent);



        // copy over the values
        ret.setDown(msg->control().down());
        ret.setUp(msg->control().up());
        ret.setLeft(msg->control().left());
        ret.setRight(msg->control().right());
        ret.dirty_ = true;


        delete msg;
        return ret;

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
            std::cout << "no entity for this control input of netid = " << msg->control().netid() << std::endl;
            return entt::null;
        }

        auto name = data.m_reg.get<ComponentView::clientName>(ent).name();
        auto pos = data.m_reg.get<ComponentView::position>(ent);
        std::cout << "Handling control for netid: " << msg->control().netid() << " With entt id: " << (int) ent << " And login name = "  << name <<std::endl;
        pos.print();
 


        // copy over the values
        ret->setDown(msg->control().down());
        ret->setUp(msg->control().up());
        ret->setLeft(msg->control().left());
        ret->setRight(msg->control().right());
        ret->dirty_ = true; // ADDED LATER


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


    void readPosComp(GameData::GameData& data, ProtoMessaging::UpdateEntityMessage& msg, int index)
    {

        auto comp = msg.positioncomps().Get(index);
        auto entity = TranslationSystem::getEntity(data, comp.netid());

        std::cout << "Reading position component for entity " << comp.netid() << std::endl;


        // If entity already has this component, get, if not, emplace
        auto& pos = (data.m_reg.has<ComponentView::position>(entity)) ? data.m_reg.get<ComponentView::position>(entity) :
            data.m_reg.emplace<ComponentView::position>(entity, true, true);

        // std::cout << "Got position component" << std::endl;

 //        auto ref = comp.GetReflection();

   //      const google::protobuf::Descriptor* positiondescriptor = comp.GetDescriptor();
    //     const google::protobuf::FieldDescriptor* prevx_field = positiondescriptor->FindFieldByName("prevx");
     //    const google::protobuf::FieldDescriptor* prevy_field = positiondescriptor->FindFieldByName("prevy");
     //    const google::protobuf::FieldDescriptor* curx_field = positiondescriptor->FindFieldByName("curx");
     //    const google::protobuf::FieldDescriptor* cury_field = positiondescriptor->FindFieldByName("cury");

         // TODO: what if i dont set some value?

        pos.setPrevx(comp.prevx());
        //  std::cout << "prevx = " << comp.prevx() << std::endl;
        pos.setPrevy(comp.prevy());
        //  std::cout << "prevy = " << comp.prevy() << std::endl;
        pos.setCurx(comp.curx());
        //  std::cout << "curx = " << comp.curx() << std::endl;
        pos.setCury(comp.cury());
        // std::cout << "cury = " << comp.cury() << std::endl;
        std::cout << "Receive: curX = " << comp.curx() << " curY = " << comp.cury() << std::endl;
        pos.networked = true;
    }

    void readObjComp(GameData::GameData& data, ProtoMessaging::UpdateEntityMessage& msg, int index)
    {
        auto comp = msg.mapobjectcomps().Get(index);
        // get the entity in the register, and swap over the values
        auto entity = TranslationSystem::getEntity(data, comp.netid());

        std::cout << "Reading mapobject comp for entity " << comp.netid() << std::endl;


        auto& obj = (data.m_reg.has<ComponentView::mapObject>(entity)) ? data.m_reg.get<ComponentView::mapObject>(entity) :
            data.m_reg.emplace<ComponentView::mapObject>(entity, true);


        //   auto ref = comp.GetReflection();
        //   const google::protobuf::Descriptor* objectdescriptor = comp.GetDescriptor();
        //   const google::protobuf::FieldDescriptor* mapchar_field = objectdescriptor->FindFieldByName("mapChar");
        //   std::cout << "comp = " << comp.mapchar()[0] << std::endl;
        obj.setMapChar(comp.mapchar()[0]);

        obj.networked = true;
    }

    void readNameComp(GameData::GameData& data, ProtoMessaging::UpdateEntityMessage& msg, int index)
    {
        auto comp = msg.clientnamecomps().Get(index);
        auto entity = TranslationSystem::getEntity(data, comp.netid());

        std::cout << "Reading name comp for entity " << comp.netid() << std::endl;
        //  std::cout << "entity id = " << (int) entity << std::endl;
          // get the entity in the register, and swap over the values
        auto& name = (data.m_reg.has<ComponentView::clientName>(entity)) ? data.m_reg.get<ComponentView::clientName>(entity) :
            data.m_reg.emplace<ComponentView::clientName>(entity, comp.name(), true);

        //    auto ref = comp.GetReflection();
          //  const google::protobuf::Descriptor* namedescriptor = comp.GetDescriptor();
           // const google::protobuf::FieldDescriptor* name_field = namedescriptor->FindFieldByName("name");
         //   std::cout << "name = " << comp.name() << std::endl;
        name.setName(comp.name());

        name.networked = true;
    }

    void readControlComp(GameData::GameData& data, ProtoMessaging::UpdateEntityMessage& msg, int index)
    {
        auto comp = msg.uinputcomps().Get(index);
        auto entity = TranslationSystem::getEntity(data, comp.netid());

        std::cout << "Reading uinput comp for entity " << comp.netid() << std::endl;
        //   std::cout << "entity id = " << (int)entity << std::endl;

           // If entity already has this component, get, if not, emplace
        auto& uin = (data.m_reg.has<ComponentView::userInput>(entity)) ? data.m_reg.get<ComponentView::userInput>(entity) :
            data.m_reg.emplace<ComponentView::userInput>(entity, true);

        //   auto ref = comp.GetReflection();

       //    const google::protobuf::Descriptor* inputdescriptor = comp.GetDescriptor();
       //    const google::protobuf::FieldDescriptor* up_field = inputdescriptor->FindFieldByName("up");
       //    const google::protobuf::FieldDescriptor* down_field = inputdescriptor->FindFieldByName("down");
       //    const google::protobuf::FieldDescriptor* left_field = inputdescriptor->FindFieldByName("left");
       //    const google::protobuf::FieldDescriptor* right_field = inputdescriptor->FindFieldByName("right");

        uin.setLeft(comp.left());
        uin.setRight(comp.right());
        uin.setDown(comp.down());
        uin.setUp(comp.up());
        uin.networked = true;

    }

}