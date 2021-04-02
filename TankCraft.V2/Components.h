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
	private:
		bool up_;
		bool down_;
		bool left_;
		bool right_;

	public:
		userInput_() { up_ = false; down_ = false; left_ = false; right_ = false; dirty = false; }
		~userInput_() = default;
		bool up() { return up_; }
		bool down() { return down_; }
		bool left() { return left_; }
		bool right() { return right_; }

		void setUp(bool set) { up_ = set; dirty = true; }
		void setDown(bool set) { down_ = set; dirty = true; }
		void setLeft(bool set) { left_ = set; dirty = true; }
		void setRight(bool set) { right_ = set; dirty = true; }

		virtual void write(ProtoMessaging::UpdateEntityMessage& message, networkID netid) override;

	} userInput;


	typedef struct position_ : baseComponent {
	private:
		uint32_t prevx_;
		uint32_t prevy_;
		uint32_t curx_;
		uint32_t cury_;

	public:
		position_() { prevx_ = 0; prevy_ = 0; curx_ = 0; cury_ = 0; dirty = false; }
		position_(bool spawn) {
			// set position at random value
			srand(time(NULL));
			curx_ = rand() % WIDTH;
			cury_ = rand() % HEIGHT;
			prevx_ = 0;
			prevy_ = 0;
		}
		
		uint32_t prevx() { return prevx_; }
		uint32_t prevy() { return prevy_; }
		uint32_t curx() { return curx_; }
		uint32_t cury() { return cury_; }

		void setPrevx(uint32_t set) { prevx_ = set; dirty = true; }
		void setPrevy(uint32_t set) { prevy_ = set; dirty = true; }
		void setCurx(uint32_t set) { curx_ = set; dirty = true; }
		void setCury(uint32_t set) { cury_ = set; dirty = true; }

		virtual void write(ProtoMessaging::UpdateEntityMessage& message, networkID netid) override;
	} position;


	typedef struct mapObject_ : baseComponent {
	private:
		char mapChar_;

	public:
		mapObject_() { mapChar_ = 'X'; dirty = false; }
		mapObject_(char newChar) { mapChar_ = newChar; dirty = false; }

		char mapChar() { return mapChar_; }
		void setMapChar(char set) { mapChar_ = set; dirty = true; }
		virtual void write(ProtoMessaging::UpdateEntityMessage& message, networkID netid) override;
	} mapObject;

	typedef struct score_ : baseComponent {
	private:
		uint32_t points_;

	public:
		score_() { points_ = 0; dirty = false; }
		uint32_t points() { return points_; }
		void setPoints(uint32_t set) { points_ = set; dirty = true; }
		// move constructor 
		score_(struct score_&& other) : points_{ other.points_ } { other.points_ = 0; };
		
		// move assignment
		struct score_& operator=(struct score_&& other)  {
			if (this != &other) {
				points_ = other.points_;
			}
			return *this;
		}
		virtual void write(ProtoMessaging::UpdateEntityMessage& message, networkID netid) override;
	} score;

	typedef struct clientName_ : baseComponent 
	{
	private:
		std::string name_;
	public:
		clientName_() { name_ = ""; }
		clientName_(std::string name_) { name_ = name_; }
		std::string name() { return name_; }
		void setName(std::string set) { name_ = set; dirty = true; }
		virtual void write(ProtoMessaging::UpdateEntityMessage& message, networkID netid) override;
	} clientName;


	// Adding health to the character
	typedef struct health_ : baseComponent {
	private:
		int32_t hp_;
	public:
		int32_t hp() { return hp_; }
		void setHp(int32_t set) { hp_ = set; dirty = false; }
		health_() { hp_ = 100; }
	} health;

	// TODO: implement getters and setters, havent done it yet since this might be phased out
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
		//bool isNetwoked; // If the entity should be networked
		virtual void write(ProtoMessaging::UpdateEntityMessage& message, networkID netid) override;
	} networked;
}