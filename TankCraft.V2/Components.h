#pragma once
#include <msgpack.hpp>
#include <list>
#include "UISystem.h"
#include "BaseComponent.h"

/*
* Header for all of the games components
*
*/

/*
* 1) Component.write writes the component to an update packet. (to be called in the component update list)
* 2) To update the component, add a read function?? 
*/


namespace ComponentView {
	typedef enum  {
		nullComp, // if the component is some null value
		Position,
		MapObject,
		Score,
		ClientName,
		DirtyClient,
		Health,
		DamageDone,
		input
	} ComponentID;


	// lets try with only this one
	typedef struct userInput_ : baseComponent {
		bool up;
		bool down;
		bool left;
		bool right;
		bool dirty;
		userInput_() { up = 0; down = 0; left = 0; right = 0; dirty = 0; }

		virtual void write(ProtoMessaging::UpdateEntityMessage& message, networkID netid) override;
	} userInput;


	typedef struct position_ : baseComponent {
		uint32_t prevx;
		uint32_t prevy;

		uint32_t curx;
		uint32_t cury;
		position_() { prevx = 0; prevy = 0; curx = 0; cury = 0; }

		position_(bool spawn) {
			// set position at random value
			srand(time(NULL));
			curx = rand() % WIDTH;
			cury = rand() % HEIGHT;
			prevx = 0;
			prevy = 0;
		}
		virtual void write(ProtoMessaging::UpdateEntityMessage& message, networkID netid) override;
	} position;


	typedef struct mapObject_ : baseComponent {
		char mapChar;
		mapObject_() { mapChar = 'X'; }
		mapObject_(char newChar) { mapChar = newChar; }
		virtual void write(ProtoMessaging::UpdateEntityMessage& message, networkID netid) override;
	} mapObject;

	typedef struct score_ : baseComponent {
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
		virtual void write(ProtoMessaging::UpdateEntityMessage& message, networkID netid) override;
	} score;

	typedef struct clientName_ : baseComponent {
		std::string name;
		clientName_() { name = ""; }
		clientName_(std::string name_) { name = name_; }
		virtual void write(ProtoMessaging::UpdateEntityMessage& message, networkID netid) override;
	} clientName;


	// Adding health to the character
	typedef struct health_ : baseComponent {
		int32_t hp;
		health_() { hp = 100; }
	} health;

	typedef struct damageDone_ : baseComponent {
		int32_t damage;
		damageDone_() { damage = 0; }
		damageDone_(int32_t dm) { damage = dm; }
		virtual void write(ProtoMessaging::UpdateEntityMessage& message, networkID netid) override;
	} damageDone;

	// To signify if the entity is networked.
	// Could be filled with additional network important information, but for now only filled with a boolean
	typedef struct networked_ : baseComponent {
		//std::list<component&> components; // List of components that are networked		// TODO: may need to pair the component with a type
		//long clientID;
		bool isNetwoked; // If the entity should be networked
		virtual void write(ProtoMessaging::UpdateEntityMessage& message, networkID netid) override;
	} networked;
}