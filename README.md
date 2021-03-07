# TankCraft.V2
This is a prototype application of a networked ECS system with EnTT and RakNet implementation.

## The Game

The game is an executable file.

## Entity

All entities are stored inside the registry located within the SceneData class.

## Component

Components.h file contains the definition of all components regarding the tank game.
FreeListComponent.h file contains definition of components for the ID Translation System.

## System

The current list of systems:

- IDTranslationSystem
- NetworkSystem
- MovementSystem
- UISystem

### IDTranslationSystem



### NetworkSystem

The network system currently handles all the networking of the game. It has the following main functionalities:

1. Establish client-server connections using a peer-to-peer interface provided by RakNet

2. Update function for both client and server. The update function will try to receive a packet from the peer and recognize the packet type. Then, based on the packet type, the update function calls corresponding helper functions to handle each case.

3. Helper functions: We currently have helper functions for different connection status and entity operations. Many helper functions are divided into server and client cases. For example, the add entity function for server

   		if (isServer) {
   			// add the entity with entity id given to the m_reg
   			auto newEntity = data.m_reg.create();
   	
   			// Allocate a new netId for this entity
   			networkID netid = transSystem.createMapping(data, newEntity);
   			
   			// Append into the client entity map
   			data.clientAddressToEntities[pack->systemAddress].push_back(netid);
   	
   			// create a new add entity packet
   			Packets::addEntityPacket addpack = Packets::addEntityPacket(netid);
   	
   			//data.clientAddressToEntities[pack->systemAddress].push_back(netid);
   	
   			std::cout << "Registered entity from client " << pack->systemAddress.GetPort() << ", broadcasting..." << std::endl;
   			// broadcast to all clients to add a new entity with entity ID, and all components
   			data.rpi->Send(reinterpret_cast<char*>(&addpack), 
   				sizeof(addpack), 
   				HIGH_PRIORITY, 
   				RELIABLE_ORDERED, 
   				0, 
   				pack->systemAddress, 
   				true);
   The add entity function for client

   		}
   		else {
   			// If is client:
   			// if client doesnt have an entity with this netid 
   			// add the entity with the entity id given to the m_reg
   	
   			// if were not requesting, but responding to a request
   			if (!initial) {
   				std::cout << "Adding entity in client" << std::endl;
   				auto newEntity = data.m_reg.create();
   	
   				// cast the input packet to an add entity packet
   				Packets::addEntityPacket* addpack = (Packets::addEntityPacket*)pack->data;
   	
   				// when unpacking input packet, get the netID passed to us
   				// Create a new mapping of netID and entity
   				transSystem.setMapping(data, addpack->netID, newEntity);
   			}
   			else {
   				// if were requesting:
   				// addpacket request
   				Packets::addEntityPacket addpack = Packets::addEntityPacket(0);
   				std::cout << "Sending out add entity request packet to server" << std::endl;
   				// Request an addition of a new entity
   				data.rpi->Send(reinterpret_cast<char*>(&addpack),
   					sizeof(addpack),
   					HIGH_PRIORITY,
   					RELIABLE_ORDERED,
   					0,
   					data.rakAddress,
   					false);
   			}

### MovementSystem

The movement system has nothing inside yet. We plan to have this system manages the user input instead of the UI system.



### UISystem

UI system manages the UI of the game, which is essentially printing to the console right now.

It also has a function that listen to the keyboard input so that the user can control the tank. However, this functionality will block the refreshing of the console. We would currently disable the UI to display debug messages on clients.