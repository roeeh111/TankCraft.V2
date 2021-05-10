#include <MessageIdentifiers.h>
#include "NetworkUtilitySystem.h"
#include "IDTranslationSystem.h"
#include "BitStream.h"
#include "RegWrappers.h"
#include "MessagingSystem.h"
#include "MovementSystem.h"

namespace NetworkUtilitySystem {

	// Change this, so that we get the control input of the given user, and change that 
	void handleControl(GameData::GameData& data, RakNet::Packet* pack)
	{
		// given a packet, call readcontrol
		ComponentView::userInput contrl;
		std::string stream = std::string((char*)(pack->data + 1));
	//	entt::entity ent;
		entt::entity ent = MessagingSystem::readControls(data, stream, &contrl);

		//ComponentView::userInput& contrl = MessagingSystem::readControls(data, stream, &ent);


		if (!(ent == entt::null)) {
			//	 if readcontrol returns not nullentitiy, call movementsystem.move(component) 
			MovementSystem::MovementSystem movementSystem;
			movementSystem.moveEntity(data, ent, contrl);
		}
	}

	void sendControl(GameData::GameData& data, ComponentView::userInput& usrInput, networkID netid)
	{
		RakNet::BitStream stream = RakNet::BitStream();
		MessagingSystem::writeControls(stream, usrInput, netid);	// PROBLEM ON THIS LINE!!
		data.rpi->Send(&stream,
			HIGH_PRIORITY,
			RELIABLE_ORDERED,
			0,
			data.rakAddress,
			false);
	}

	entt::entity addEntity(GameData::GameData& data, RakNet::Packet* pack, bool isServer, bool responding)
	{
		// If is Server: (we dont need to examine the netid/timestamp of the incomming packet. So make a new message object and sendit)
		if (isServer) {

			// add the entity with entity id given to the m_reg
			auto newEntity = RegWrapper::createEntity(data.m_reg, true);

			// Allocate a new netId for this entity (make sure that this entity doesnt already exist...)
			networkID netid = TranslationSystem::createMapping(data, newEntity);

			// If the client map isnt constructed for this address, construct it
			if (!data.clientAddressToEntities.count(pack->systemAddress)) {
				data.clientAddressToEntities[pack->systemAddress] = std::list<networkID>();
			}

			// Append into the client entity map
			data.clientAddressToEntities[pack->systemAddress].push_back(netid);

			// create a new add entity packet
			RakNet::BitStream stream = RakNet::BitStream();
			MessagingSystem::writeAddEntity(stream, netid);


			//std::cout << "Registered entity from client " << pack->systemAddress.GetPort() << ", broadcasting..." << std::endl;
			//std::cout << "Adding entity for netid " << netid << std::endl;
			// broadcast to all clients to add a new entity with entity ID, and all components

			if (responding) {	// Tell all clients to add this entity
				data.rpi->Send(&stream,
					HIGH_PRIORITY,
					RELIABLE_ORDERED,
					0,
					RakNet::UNASSIGNED_SYSTEM_ADDRESS, //				pack->systemAddress, 
					true);
			}
			else {
				data.rpi->Send(&stream, // tell all clients excepy for whoever sent us this message to add this entity
					HIGH_PRIORITY,
					RELIABLE_ORDERED,
					0,
					pack->systemAddress,
					true);
			}
			//std::cout << "Finished broadcasting" << std::endl;
			return newEntity;

		}
		else {
			// If is client:
			// if were not requesting, but responding to a request
			if (!responding) {
				//std::cout << "Adding entity in client after receiving a packet" << std::endl;

				std::string str = std::string((char*)(pack->data + 1), pack->length - 1);

				ProtoMessaging::AddRemoveEntityMessage* msg = MessagingSystem::readAddRemoveEntity(str);

				auto newEntity = TranslationSystem::addEntity(data, msg);
				std::cout << "Client adding entity with netid " << msg->netid() << "for entity id = " << (int)newEntity << std::endl;
				//std::cout << "Timestamp = " << msg->timestamp() << std::endl;

				delete msg;
				return newEntity;
			}
			else {
				// if were requesting:
				//std::cout << "Sending out add entity request packet to server" << std::endl;
				RakNet::BitStream stream = RakNet::BitStream();

				MessagingSystem::writeAddEntity(stream, 0);


				// Request an addition of a new entity
				data.rpi->Send(&stream,
					HIGH_PRIORITY,
					RELIABLE_ORDERED,
					0,
					data.rakAddress,
					false);
				return entt::null;
			}
		}
	}

	// SO! there is a problem with remove entity....
	void removeEntity(GameData::GameData& data, RakNet::Packet* pack, networkID remID, bool isServer, bool responding)
	{

		// If is Server:
		if (isServer) {
			if (responding) { // if were responding to some message
				// Get the remove packet
				std::string str = std::string((char*)(pack->data + 1));
				ProtoMessaging::AddRemoveEntityMessage* msg = MessagingSystem::readAddRemoveEntity(str);

				std::cout << "Server removing entity " << msg->netid() << std::endl;

				// Remove the given entity from the m_reg
				if (TranslationSystem::hasMapping(data, msg->netid())) {
					std::cout << "Server removing entity " << msg->netid() << std::endl;
					data.m_reg.destroy(TranslationSystem::getEntity(data, msg->netid()));
				}
				else {
					return;
				}

				// remove the mapping from the client map
				data.clientAddressToEntities[pack->systemAddress].remove(msg->netid());

				// Free the netid from the translation system
				TranslationSystem::freeID(data, msg->netid());

				// Write the packet data back to the stream
				RakNet::BitStream stream = RakNet::BitStream();

				stream.Write((char*)pack->data, pack->length);


				std::cout << "Server removed entity from client " << pack->systemAddress.GetPort() << ", broadcasting..." << std::endl;
				// broadcast to all clients to add a new entity with entity ID, and all components
				data.rpi->Send(&stream,
					HIGH_PRIORITY,
					RELIABLE_ORDERED,
					0,
					RakNet::UNASSIGNED_SYSTEM_ADDRESS, // pack->systemAddress,
					true);
				//std::cout << "sent message" << std::endl;
				delete msg;
			}
			else {
				RakNet::BitStream stream = RakNet::BitStream();

				MessagingSystem::writeRemoveEntity(stream, remID);

				if (TranslationSystem::hasMapping(data, remID)) {
					std::cout << "Server removing entity " << remID << std::endl;
					data.m_reg.destroy(TranslationSystem::getEntity(data, remID));
				}

				// remove the mapping from the client map
				data.clientAddressToEntities[pack->systemAddress].remove(remID);

				// Free the netid from the translation system
				TranslationSystem::freeID(data, remID);


				// remove the entitiy by broadcasting a remove
				data.rpi->Send(&stream,
					HIGH_PRIORITY,
					RELIABLE_ORDERED,
					0,
					data.rakAddress,
					true);
			}
		}
		else {
			// If is client:
			// if were not requesting, but responding to a request
			if (responding) {
				//std::cout << "removing entity in client after receiving a packet" << std::endl;

				std::string str = std::string((char*)(pack->data + 1), pack->length - 1);
				ProtoMessaging::AddRemoveEntityMessage* msg = MessagingSystem::readAddRemoveEntity(str);

				// Remove the given entity from the m_reg
				if (TranslationSystem::hasMapping(data, msg->netid())) {
					data.m_reg.destroy(TranslationSystem::getEntity(data, msg->netid()));
				}
				else {
					return;
				}

				// Free the netid from the translation system
				TranslationSystem::freeID(data, msg->netid());
				delete msg;
			}
			else {
				// if were requesting:
			//	std::cout << "Sending out remove entity request packet to server" << std::endl;
				RakNet::BitStream stream = RakNet::BitStream();

				MessagingSystem::writeRemoveEntity(stream, remID);


				// Request an addition of a new entity
				data.rpi->Send(&stream,
					HIGH_PRIORITY,
					RELIABLE_ORDERED,
					0,
					data.rakAddress,
					false);
			}
		}
	}

	void sendClientInput(GameData::GameData& data, RakNet::Packet* pack) {
		//std::cout << "Sending out client input to the server" << std::endl;
		RakNet::BitStream stream = RakNet::BitStream();

		// HERE we write the control into the stream
	//	MessagingSystem::writeControls(stream, 0);


		// Request an addition of a new entity
		data.rpi->Send(&stream,
			HIGH_PRIORITY,
			RELIABLE_ORDERED,
			0,
			data.rakAddress,
			false);
	}


	void NetworkUtilitySystem::broadcast(GameData::GameData& data, const RakNet::BitStream* bitStream, PacketPriority priority, PacketReliability reliability, char orderingChannel)
	{
		for (auto& it : data.clientAddressToEntities) {
			data.rpi->Send(bitStream,
				priority,
				reliability,
				orderingChannel,
				it.first,
				false);
		}

	}

	void handleGameUpdate(GameData::GameData& data, RakNet::Packet* pack)
	{
		std::string stream = std::string((char*)(pack->data + 1));
	
 		/* Protobuf method of doing it
		MessagingSystem::MessagingSystem messagingSystem;
		messagingSystem.readGameUpdate(data, stream);
		*/
	}



}


/* Commented out remove entity

		// If is Server:
		if (isServer) {
			// Get the remove packet
			std::string str = std::string((char*)(pack->data + 1));
			ProtoMessaging::AddRemoveEntityMessage* msg = MessagingSystem::readAddRemoveEntity(str);

			std::cout << "Server removing entity " << msg->netid() << std::endl;

			// Remove the given entity from the m_reg
			if (TranslationSystem::hasMapping(data, msg->netid())) {
				std::cout << "Server removing entity " << msg->netid() << std::endl;
				data.m_reg.destroy(TranslationSystem::getEntity(data, msg->netid()));
			}
			else {
				return;
			}

			// remove the mapping from the client map
			data.clientAddressToEntities[pack->systemAddress].remove(msg->netid());

			// Free the netid from the translation system
			TranslationSystem::freeID(data, msg->netid());

			// Write the packet data back to the stream
			RakNet::BitStream stream = RakNet::BitStream();

			// TODO: this might not work!
			stream.Write((char*)pack->data, pack->length);


			std::cout << "Server removed entity from client " << pack->systemAddress.GetPort() << ", broadcasting..." << std::endl;
			// broadcast to all clients to add a new entity with entity ID, and all components
			data.rpi->Send(&stream,
				HIGH_PRIORITY,
				RELIABLE_ORDERED,
				0,
				pack->systemAddress,
				true);
			//std::cout << "sent message" << std::endl;
			delete msg;
		}
		else {
			// If is client:
			// if were not requesting, but responding to a request
			if (responding) {
				//std::cout << "removing entity in client after receiving a packet" << std::endl;

				std::string str = std::string((char*)(pack->data + 1));
				ProtoMessaging::AddRemoveEntityMessage* msg = MessagingSystem::readAddRemoveEntity(str);

				// Remove the given entity from the m_reg
				if (TranslationSystem::hasMapping(data, msg->netid())) {
					data.m_reg.destroy(TranslationSystem::getEntity(data, msg->netid()));
				}
				else {
					return;
				}

				// Free the netid from the translation system
				TranslationSystem::freeID(data, msg->netid());
				delete msg;
			}
			else {
				// if were requesting:
				//std::cout << "Sending out add entity request packet to server" << std::endl;
				RakNet::BitStream stream = RakNet::BitStream();

				MessagingSystem::writeRemoveEntity(stream, remID);


				// Request an addition of a new entity
				data.rpi->Send(&stream,
					HIGH_PRIORITY,
					RELIABLE_ORDERED,
					0,
					data.rakAddress,
					false);
			}
		}




*/


