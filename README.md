# TankCraft.V2
This is a prototype application of a networked ECS system with EnTT and RakNet implementation.



## Setup Instruction

#### Dependencies with vcpkg

We use [vcpkg](https://docs.microsoft.com/en-us/cpp/build/vcpkg?view=msvc-160) to install all dependencies so that we don't have different directory paths.

[Install vcpkg](https://docs.microsoft.com/en-us/cpp/build/install-vcpkg?view=msvc-160&tabs=windows)

[Integrate vcpkg to Visual Studio](https://docs.microsoft.com/en-us/cpp/build/integrate-vcpkg?view=msvc-160)

In the project properties -> Configuration Properties -> vcpkg, we can choose to use the vcpkg.

We will install EnTT and ProtoBuffer

```
vcpkg install entt
vcpkg install protobuf protobuf:x64-windows
vcpkg integrate install
```

It's known that at compilation, ProtoBuffer will throw lots of warnings and [2 errors regarding the constexpr](https://github.com/protocolbuffers/protobuf/issues/8268), but these don't affect the build.

#### RakNet

As for RakNet, we keep the source files in the same directory:

`C:\RakNet\Source;`

This directory needs to be added to  Configuration Properties -> C/C++ -> Additional Include Directories

The RakNet source file also need to be manually added to that directory.

If there are issues with RakNet compilation, please refer to http://www.jenkinssoftware.com/raknet/manual/tutorial.html.



Since RakNet is local, we have a few Message identifiers that need to be manually added to `MessageIdentifiers.h` of the RakNet source.

The file can be quick accessed by going to the `[NetworkSystem.cpp](https://github.com/roeeh111/TankCraft.V2/blob/master/TankCraft.V2/NetworkSystem.cpp)` and click on any message types.

Starting at line 429 of `MessageIdentifiers.h` , replace:

	// For the user to use.  Start your first enumeration at this value.
	ADD_ENTITY,
	REMOVE_ENTITY,
	UPDATE_ENTITY,
	CONTROL,
	LOGIN,
	ID_USER_PACKET_ENUM
#### Other

We are mostly using the local windows debugger to test.

There might be other issues that we missed, please let us know.

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

The entity ID is a random uint32 stored in the registry. The creation of such ID cannot be easily managed to be generated as the same across all clients. Thus if we want to locate a specific entity, we would need a unified identifier for communication process.

The ID translation system would always create mapping from an entity ID to a network ID during the creation of an entity. The network ID is always assigned by the server and then broadcasted to all clients. 

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
   	
   			//data.clientAddressToEntities[pack->systemAddress].push_back(netid);
   			
   			// create a new add entity packet
   			Packets::addEntityPacket addpack = Packets::addEntityPacket(netid);
   			
   			// write the serialized data into the bitstream
   			MessagingSystem::writeAddEntity(stream, netid);
   	
   			std::cout << "Registered entity from client " << pack->systemAddress.GetPort() << ", broadcasting..." << std::endl;
   			// broadcast to all clients to add a new entity with specific network ID, and all components
   			data.rpi->Send(reinterpret_cast<char*>(&addpack), 
   				sizeof(addpack), 
   				HIGH_PRIORITY, 
   				RELIABLE_ORDERED, 
   				0, 
				pack->systemAddress, 
   				true);
   		}
   The add entity function for client
   
   		else {
   			if (responding) {
   				// Client receiving the add entity request
   	
   				auto newEntity = RegWrapper::createEntity(data.m_reg, true);
   	
   				std::string str = std::string((char*) (pack->data + 1));		
   				
   				ProtoMessaging::AddRemoveEntityMessage* msg = MessagingSystem::readAddRemoveEntity(str);
   	
   				// when unpacking input packet, get the netID passed to us
   				// Create a new mapping of netID and entity
   				TranslationSystem::setMapping(data, msg->netid(), newEntity);
   				delete msg;
   			}
   			else {
   				// Client sending the add entity request
   				
   				Packets::addEntityPacket addpack = Packets::addEntityPacket(0);
   	
   				// Request an addition of a new entity
   				data.rpi->Send(reinterpret_cast<char*>(&addpack),
   					sizeof(addpack),
   					HIGH_PRIORITY,
   					RELIABLE_ORDERED,
   					0,
   					data.rakAddress,
   					false);
   			}

The unresolved problem within this add entity function is that currently in our game we don't have many types of entities. We need to find a way to generalize this add entity function and figure out how to send component information without having to predefine it.

### MovementSystem

The movement system would advance the movement of all entities containing the movement component. Working in progress.



### UISystem

UI system manages the UI of the game, which is essentially printing to the console right now.

It also has a function that listen to the keyboard input so that the user can control the tank. However, this functionality will block the refreshing of the console. We would currently disable the UI to display debug messages on clients.