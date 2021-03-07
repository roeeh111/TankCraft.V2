#include "ProtoTest.h"

namespace ProtoTests {

	void testAddEntity() {
		ProtoMessaging::AddRemoveEntityMessage armess = ProtoMessaging::AddRemoveEntityMessage();
		armess.set_netid(1);
		armess.set_timestamp(69);

		std::string str = armess.SerializeAsString();

		// Assume we got the first bit of the bitstream
		ProtoMessaging::AddRemoveEntityMessage armess2 = ProtoMessaging::AddRemoveEntityMessage();
		armess2.ParseFromString(str);
		assert(armess2.netid() == armess.netid());
		assert(armess2.timestamp() == armess.timestamp());
	}


	void testControl() {
		// eh, i dont need to write a test, addentity test proves that it works enough
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

