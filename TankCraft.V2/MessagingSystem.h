#pragma once
#include "Components.h"
#include "RakNetTypes.h"
#include "BitStream.h"
#include "Tanks.pb.h"
#include "IDTranslationComponent.h"
#include "BaseComponent.h"

#define FIRST_COMPONENT_IN_UPDATE 2

/*
*  System for creating, serializing (maybe sending?) network messages
*  
*/


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


// Doesnt hold any state data, so just a namespace for now
namespace MessagingSystem {

	// given a bitstream and the ToUpdate map, write a gameupdate packet to the bitstream
	void writeGameUpdate(RakNet::BitStream &stream, std::map<networkID, std::list<baseComponent>>& updateMap); // add refrence to list of components to update

	// Given a bitstream and a netid, write a addEntity packet to the bitstream
	void writeAddEntity(RakNet::BitStream& stream, networkID netid);

	// Given a bitstream and a netid, write a removeEntity packet to the bitstream
	void writeRemoveEntity(RakNet::BitStream& stream, networkID netid);

	// Given a bitstream and a controls component, write a controlsPacket to the bitstream
	void writeControls(RakNet::BitStream& stream, ComponentView::userInput control, networkID netid);

	// Given a bitstream, read it into a GameUpdatePacket, verify its integrity and do the actual game updates
	// (game updates can be handled by another system)
	void readGameUpdate(GameData::GameData& data, std::string& stream);

	// Given a bitstream, read it into a addRemoveEntityPacket, verify its integrity and return the message
	// (entity addition can be handled by another system)
	ProtoMessaging::AddRemoveEntityMessage* readAddRemoveEntity(std::string &stream);

	// Given a bitstream, read it into a ControlsPacket, verify its integrity and execute the controls
	entt::entity readControls(GameData::GameData& data, std::string& stream, ComponentView::userInput* ret);


	/*
	* Functions to read a protobuf component into a game component (TBH, WE CAN JUST USE PROTOBUF COMPONENTS IN THE GAME....)
	*/
	void readPosComp(GameData::GameData& data, ProtoMessaging::PositionComponent& comp);
	void readObjComp(GameData::GameData& data, ProtoMessaging::MapObjectComponent& comp);
	void readNameComp(GameData::GameData& data, ProtoMessaging::ClientNameComponent& comp);


	// Debug only
	std::string writeGameUpdate(std::map<networkID, std::list<baseComponent>> & updateMap);

}

