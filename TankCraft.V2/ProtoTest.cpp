#include "ProtoTest.h"
#include "MessagingSystem.h"
/*
	Serializing a list of something in protobuf:
	1) create a version of the "node" or "data" that is stored in the list object in proto file
	2) create a message with a netid and a repeated field of that node.
	3) write a wrapper to serialize/deserialize the list
*/

/*
	Serializing a map of something in protobuf:
	What is a map if not a list of pairs? - Roee Horowitz
	1) create a version of the key component and of the value component
	2) create a message with netid, and a repeated field that contains a single key and single value
	3) write a wrapper to serialize/deserialize the map
*/

/*
	Serializing an array of something in protubuf:
	- look into how protobuf does it, may already have a protocol for doing so
	- if not, its the same as a list, except the deserialize wrapper writes into an array rather than a list
*/

namespace ProtoTests {

	void testOptionalFields()
	{

		auto comp = new ProtoMessaging::PositionComponent;

		auto pos = new ComponentView::position();
		//pos->setCurx(0);
		//pos->setCury(1);
		//pos->setPrevx(2);
		//pos->setPrevy(3);
		comp->set_curx(0);
		comp->set_cury(1);
		comp->set_prevx(2);
		comp->set_prevy(3);
		std::cout << "curx = " << comp->curx() << ", cury = " << comp->cury() << ", prevx = " << comp->prevx() << ", prevy = " << comp->prevy() << std::endl;

		auto dstring = comp->SerializeAsString();
		std::cout << "dstring size = " << dstring.size() << std::endl;
		comp->ParseFromString(dstring);


		if (comp->has_prevx()) {
			std::cout << "has prevx" << std::endl;
			pos->setPrevx(comp->prevx());
		}
		//  std::cout << "prevx = " << comp.prevx() << std::endl;
		if (comp->has_prevy()) {
			std::cout << "has prevy" << std::endl;
			pos->setPrevy(comp->prevy());
		}
		//  std::cout << "prevy = " << comp.prevy() << std::endl;
		if (comp->has_curx()) {
			std::cout << "has curx" << std::endl;
			pos->setCurx(comp->curx());
		}
		//  std::cout << "curx = " << comp.curx() << std::endl;
		if (comp->has_cury()) {
			std::cout << "has cury" << std::endl;
			pos->setCury(comp->cury());
		}
		// std::cout << "cury = " << comp.cury() << std::endl;

		std::cout << "curx = " << pos->curx() << ", cury = " << pos->cury() << ", prevx = " << pos->prevx() << ", prevy = " << pos->prevy() << std::endl;



		auto comp2 = new ProtoMessaging::PositionComponent;
		comp2->set_cury(1);
		comp2->set_prevx(2);
		std::cout << "curx = " << comp2->curx() << ", cury = " << comp2->cury() << ", prevx = " << comp2->prevx() << ", prevy = " << comp2->prevy() << std::endl;

		auto dstring2 = comp2->SerializeAsString();
		std::cout << "dstring2 size = " << dstring2.size() << std::endl;
		comp2->ParseFromString(dstring2);


		auto pos2 = new ComponentView::position();

		if (comp2->has_prevx()) {
			std::cout << "has prevx" << std::endl;
			pos2->setPrevx(comp2->prevx());
		}
		//  std::cout << "prevx = " << comp.prevx() << std::endl;
		if (comp2->has_prevy()) {
			std::cout << "has prevy" << std::endl;
			pos2->setPrevy(comp2->prevy());
		}
		//  std::cout << "prevy = " << comp.prevy() << std::endl;
		if (comp2->has_curx()) {
			std::cout << "has curx" << std::endl;
			pos2->setCurx(comp2->curx());
		}
		//  std::cout << "curx = " << comp.curx() << std::endl;
		if (comp2->has_cury()) {
			std::cout << "has cury" << std::endl;
			pos2->setCury(comp2->cury());
		}
		// std::cout << "cury = " << comp.cury() << std::endl;

		std::cout << "curx = " << pos2->curx() << ", cury = " << pos2->cury() << ", prevx = " << pos2->prevx() << ", prevy = " << pos2->prevy() << std::endl;





		delete comp;
		delete pos;
		delete comp2;
		delete pos2;
	}


	void serializationOverheadTest()
	{
	}


	void gameUpdateOverheadTest(int iterations) {
		std::cout << "Testing addition of "<< iterations << " full component updates" << std::endl;
		auto message = ProtoMessaging::UpdateEntityMessage();
		int size = 0;
		std::string n = "Josantonio Diaz";

		for (int i = 0; i < iterations; i++) {
			auto pos = new ComponentView::position(1, 1);
			auto input = new ComponentView::userInput(); input->setDown(4); input->setLeft(3); input->setRight(2); input->setUp(1);
			auto mapobj = new ComponentView::mapObject('x', 1);
			auto name = new ComponentView::clientName(n, 1);

			pos->write(message, 1);
			input->write(message, 1);
			mapobj->write(message, 1);
			name->write(message, 1);

			size += pos->size() + input->size() + mapobj->size() + name->size() + n.size();
		}
		auto srl = message.SerializeAsString().size();

		std::cout << "Expected size = " << size << std::endl;
		std::cout << "Serialized size = " << srl << std::endl;
		std::cout << "Difference = " << size - srl << "\n" << std::endl;
	}
	
	void testFlushRegistry(GameData::GameData& data) {
	/*	entt::entity temp;
		data.m_reg.data(); // How to get all entities in a registry

		// iterate over all entities in a registry

		data.m_reg.visit(temp, [&](const auto info) {
			auto storage = data.m_reg.storage(info);
			storage->emplace(data.m_reg, other, storage->get(entity));
			});
			*/
	}


	void testGameUpdate(GameData::GameData& data) {
		MessagingSystem::MessagingSystem messagingSystem;
		data.updateMap[0] =  std::list<baseComponent*>();

		auto pos = new ComponentView::position();
		pos->setCurx(0);
		pos->setCury(1);
		pos->setPrevx(2);
		pos->setPrevy(3);
		data.updateMap[0].push_back(pos);
		std::cout << "curx = 0, cury = 1, prevx = 2, prevy = 3" << std::endl;


		std::string str = messagingSystem.writeGameUpdate(data.updateMap);
		messagingSystem.readGameUpdate(data, str);
		delete pos;
	}

	void ProtoTests::testGameUpdate() {
		MessagingSystem::MessagingSystem messagingSystem;
		GameData::GameData data = GameData::GameData();
		data.updateMap[0] = std::list<baseComponent*>();

		auto pos = new ComponentView::position();
		pos->setCurx(0);
		pos->setCury(1);
		pos->setPrevx(2);
		pos->setPrevy(3);
		data.updateMap[0].push_back(pos);
		std::cout << "curx = 0, cury = 1, prevx = 2, prevy = 3" << std::endl;

		auto pos2 = new ComponentView::position();
		pos2->setCurx(4);
		pos2->setCury(5);
		pos2->setPrevx(6);
		pos2->setPrevy(7);
		data.updateMap[0].push_back(pos2);
		std::cout << "curx = 4, cury = 5, prevx = 6, prevy = 7" << std::endl;


		std::string str = messagingSystem.writeGameUpdate(data.updateMap);
		messagingSystem.readGameUpdate(data, str);

		delete pos;
		delete pos2;
	}


	void testAddEntity() {
		ProtoMessaging::AddRemoveEntityMessage armess = ProtoMessaging::AddRemoveEntityMessage();
		armess.set_netid(1);
		armess.set_timestamp(69);

		std::string str = armess.SerializeAsString();
		std::cout << "Size of add/remove entity message for 8 bytes = " << str.size() << std::endl;

		// Assume we got the first bit of the bitstream
		ProtoMessaging::AddRemoveEntityMessage armess2 = ProtoMessaging::AddRemoveEntityMessage();
		armess2.ParseFromString(str);
		assert(armess2.netid() == armess.netid());
		assert(armess2.timestamp() == armess.timestamp());
	}


	void testControl() {
		// eh, i dont need to write a test, addentity test proves that it works enough
		auto contmess = ProtoMessaging::ControlMessage();
		auto contrcomp = new ProtoMessaging::ControlComponent();
		contmess.set_timestamp(69);

		contrcomp->set_down(true);
		contrcomp->set_up(true);
		contrcomp->set_up(true);
		contrcomp->set_up(true);
		contmess.set_allocated_control(contrcomp);

		std::string str = contmess.SerializeAsString();
		std::cout << "Size of control message for 8 bytes = " << str.size() << std::endl;


	}

	void testUpdateEntity() {
		ProtoMessaging::UpdateEntityMessage udmess = ProtoMessaging::UpdateEntityMessage();
		// set the netid
		// set the timestamp
		udmess.set_timestamp(69);

		// add some components!
		ProtoMessaging::HealthComponent *c = udmess.add_healthcomps();
		c->set_hp(42);

		ProtoMessaging::HealthComponent* c2 = udmess.add_healthcomps();
		c2->set_hp(43);

		ProtoMessaging::PositionComponent* p1 = udmess.add_positioncomps();
		p1->set_curx(1);
		p1->set_cury(2);
		p1->set_prevx(3);
		p1->set_prevy(4);


		std::string str = udmess.SerializeAsString();
		std::cout << "Size of update entity message for 32 bytes = " << str.size() << std::endl;


		// Assume that we passed the id for an update entity, and know that we have an update entitiy
		ProtoMessaging::UpdateEntityMessage *udmess2 = new ProtoMessaging::UpdateEntityMessage();
		udmess2->ParseFromString(str);

		// get the reflection
		const auto ref = udmess2->GetReflection();

		// name the fields
		const google::protobuf::Descriptor* descriptor = udmess2->GetDescriptor();
		const google::protobuf::Descriptor* healthdescriptor = c->GetDescriptor();
		const google::protobuf::Descriptor* positiondescriptor = p1->GetDescriptor();



		// since we know that its a entityupdate, list out the fields
		const google::protobuf::FieldDescriptor* id_field = descriptor->FindFieldByName("id");
		const google::protobuf::FieldDescriptor* timestamp_field = descriptor->FindFieldByName("timestamp");
		
		const google::protobuf::FieldDescriptor* position_field = descriptor->FindFieldByName("positionComps");
		const google::protobuf::FieldDescriptor* mapobj_field = descriptor->FindFieldByName("mapObjectComps");
		const google::protobuf::FieldDescriptor* scorecomp_field = descriptor->FindFieldByName("scoreComps");
		const google::protobuf::FieldDescriptor* clientname_field = descriptor->FindFieldByName("clientNameComps");
		const google::protobuf::FieldDescriptor* healthcomp_field = descriptor->FindFieldByName("healthComps");

		const google::protobuf::FieldDescriptor* healthcomp_hp_field = healthdescriptor->FindFieldByName("hp");

		const google::protobuf::FieldDescriptor* position_prevx_field = positiondescriptor->FindFieldByName("prevx");
		const google::protobuf::FieldDescriptor* position_prevy_field = positiondescriptor->FindFieldByName("prevy");
		const google::protobuf::FieldDescriptor* position_curx_field = positiondescriptor->FindFieldByName("curx");
		const google::protobuf::FieldDescriptor* position_cury_field = positiondescriptor->FindFieldByName("cury");

		

		// Check that all of the fields 
		
		// Check the header fields
		assert(ref->GetInt64(*udmess2, timestamp_field) == 69);

		// Check fields that we havent initialized

		assert(ref->FieldSize(*udmess2, clientname_field) == 0);
		assert(ref->FieldSize(*udmess2, healthcomp_field) == 2);
		assert(ref->FieldSize(*udmess2, position_field) == 1);


		// check repeated health fields
		assert(((udmess2->healthcomps().at(0).hp())) == 42);
		assert(((udmess2->healthcomps().at(1).hp())) == 43);

		assert(((udmess2->positioncomps().at(0).prevx())) == 3);
		assert(((udmess2->positioncomps().at(0).prevy())) == 4);
		assert(((udmess2->positioncomps().at(0).curx())) == 1);
		assert(((udmess2->positioncomps().at(0).cury())) == 2);


	// Check repeated health fields through the reflection interface (reflection interface isnt working for some reason)

	//  assert((ref->GetRepeatedMessage(*udmess2, healthcomp_field, 0).GetReflection()->GetInt32(blankc, healthcomp_hp_field)) == 42);
	//	assert((ref->GetRepeatedMessage(*udmess2, healthcomp_field, 1).GetReflection()->GetInt32(blankc, healthcomp_hp_field)) == 43);


	// Check repeated position fields through the reflection interface

	//	assert((ref->GetRepeatedMessage(*udmess2, position_field, 0).GetReflection()->GetInt32(*p1, position_prevx_field)) == 3);
	//	assert((ref->GetRepeatedMessage(*udmess2, position_field, 0).GetReflection()->GetInt32(*p1, position_prevy_field)) == 4);
	//	assert((ref->GetRepeatedMessage(*udmess2, position_field, 0).GetReflection()->GetInt32(*p1, position_curx_field)) == 1);
	//	assert((ref->GetRepeatedMessage(*udmess2, position_field, 0).GetReflection()->GetInt32(*p1, position_cury_field)) == 2);
	}
}

/*
To unpack and set fields of a packet:
1) from data[0] get the packet type
2) create new UpdateEntityMessage(), or control depending on what data[0] gave us
3) call message->parsefromstring(data[1->data.length])
4) unpack and make your changes
*/

/*
To pack and send data:
1) depending on the type of message being sent, create a new protobuf message object
2) write all the information you need to that message object
3) call serialize to string on the message
4) get a new bitstream object
5) write the message type enum/char to the bitstream
6) write the serialized string to the bitstream
7) send the bitstream
*/

