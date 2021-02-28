#pragma once
#include <msgpack.hpp>
#include <list>

namespace ComponentView {

	typedef struct userInput_ {
		bool up;
		bool down;
		bool left;
		bool right;
		bool dirty;
		userInput_() { up = 0; down = 0; left = 0; right = 0; dirty = 0; }
		MSGPACK_DEFINE(up, down, left, right);
	} userInput;


	typedef struct position_ {
		uint32_t prevx;
		uint32_t prevy;

		uint32_t curx;
		uint32_t cury;
		position_() { prevx = 0; prevy = 0; curx = 0; cury = 0; }
		MSGPACK_DEFINE(prevx, prevy, curx, cury);
	//	bool dirty;
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

		// move constructor 
		score_(struct score_&& other) : points{ other.points } { other.points = 0; };
		
		// move assignment
		struct score_& operator=(struct score_&& other)  {
			if (this != &other) {
				points = other.points;
			}
			return *this;
		}
		MSGPACK_DEFINE(points);
	} score;

	typedef struct clientName_ {
		std::string name;
		clientName_() { name = ""; }
		clientName_(std::string name_) { name = name_; }
		MSGPACK_DEFINE(name);
	} clientName;


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

	// Not being used for now. Only used to distinguish whether an entity is networked.
	typedef struct networked_ {
		//RakNet::RakPeerInterface* peer; // refrence to the client/server racknet interface
		//std::list<component&> components; // List of components that are networked		// TODO: may need to pair the component with a type
		long clientID;
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
