#include "Components.h"

namespace ComponentView {
	void userInput_::clear()
	{
		left_ = false;
		right_ = false;
		up_ = false;
		down_ = false;
	}

	void userInput_::write(ProtoMessaging::UpdateEntityMessage& message, networkID netid)
	{
	//	std::cout << "in userinput write" << std::endl;
		ProtoMessaging::ControlComponent* comp = message.add_uinputcomps();
		comp->set_down(down());
		comp->set_left(left());
		comp->set_right(right());
		comp->set_up(up());
		comp->set_netid(netid);
	}


	void position_::write(ProtoMessaging::UpdateEntityMessage& message, networkID netid)
	{
	//	std::cout << "in position write" << std::endl;
		ProtoMessaging::PositionComponent* comp = message.add_positioncomps();
		comp->set_curx(curx());
		comp->set_cury(cury());
		comp->set_prevx(prevx());
		comp->set_prevy(prevy());
		comp->set_netid(netid);
	}

	void mapObject_::write(ProtoMessaging::UpdateEntityMessage& message, networkID netid)
	{
	//	std::cout << "in mapobj write" << std::endl;
		ProtoMessaging::MapObjectComponent* comp = message.add_mapobjectcomps();
		auto s = new std::string(1, mapChar());
		comp->set_allocated_mapchar(s);
		comp->set_netid(netid);

		// TODO: do i delete s????
	}

	void clientName_::write(ProtoMessaging::UpdateEntityMessage& message, networkID netid)
	{
	//	std::cout << "in clientname write" << std::endl;
		ProtoMessaging::ClientNameComponent* comp = message.add_clientnamecomps();
		comp->set_name((name()));
		comp->set_netid(netid);
	}


	void networked_::write(ProtoMessaging::UpdateEntityMessage& message, networkID netid)
	{
	}

	void score_::write(ProtoMessaging::UpdateEntityMessage& message, networkID netid)
	{
	}
}