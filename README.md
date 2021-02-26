# TankCraft.V2
This is a prototype application of a networked ECS system with EnTT and RakNet implementation.

#Entity
All entities are stored inside the registry located within the SceneData class.

#Component
Components.h file contains the definition of all components regarding the tank game.
FreeListComponent.h file contains definition of components for the ID Translation System.

#System
Currently there are this many systems:
IDTranslationSystem
NetworkSystem
SceneSystem
UISystem
