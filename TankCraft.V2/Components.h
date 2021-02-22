#pragma once
#include <msgpack.hpp>
#include <list>

namespace GameView {

	// Base struct for a component. Can be dirty or not dirty, that is all
//	typedef struct component_ {
//		bool dirty;
//	} component;

	typedef struct position_ {
		uint32_t prevx;
		uint32_t prevy;

		uint32_t curx;
		uint32_t cury;
		position_() { prevx = 0; prevy = 0; curx = 0; cury = 0; }
		MSGPACK_DEFINE(prevx, prevy, curx, cury);
	} position;


	typedef struct mapObject_ {
		char mapChar;
		mapObject_() { mapChar = 'X'; }
		mapObject_(char newChar) { mapChar = newChar; }
		MSGPACK_DEFINE(mapChar);
	} mapObject;

	typedef struct score_ {
		uint32_t points;
		score_() { points = 0; }
		MSGPACK_DEFINE(points);
	} score;

	typedef struct clientName_ {
		std::string name;
		clientName_() { name = ""; }
		clientName_(std::string name_) { name = name_; }
		MSGPACK_DEFINE(name);
	} clientName;

	// If entity has this as a component, then it is a client
	// If the dirty bit is set, for now we update the entire entity...
	//														Note: this will not be used in the networked version
	typedef struct dirtyClient_ {
		bool dirty;
		dirtyClient_() { dirty = 1; }
		MSGPACK_DEFINE(dirty);
	} dirtyClient;

	// Adding health to the character
	typedef struct health_ {
		int32_t hp;
		health_() { hp = 100; }
		MSGPACK_DEFINE(hp);
	} health;

	typedef struct damageDone_ {
		int32_t damage;
		damageDone_() { damage = 0; }
		damageDone_(int32_t dm) { damage = dm; }
		MSGPACK_DEFINE(damage);
	} damageDone;

	typedef struct networked_ {
		RakNet::RakPeerInterface* peer; // refrence to the client/server racknet interface
		//std::list<component&> components; // List of components that are networked		// TODO: may need to pair the component with a type
		// TODO: Should we have a queue of components to change????

		bool isNetwoked; // If the entity should be networked
	} networked;

	enum ComponentID {
		Position,
		MapObject,
		Score,
		ClientName,
		DirtyClient,
		Health,
		DamageDone
	};

}
