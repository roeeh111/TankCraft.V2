#pragma once
#include <msgpack.hpp>
#include <list>
#include "UISystem.h"
#include "GameData.h"
/*
* Header for all of the games components
*
*/

/*
* 1) Component.write writes the component to an update packet. (to be called in the component update list)
* 2) To update the component, add a read function?? 
*/


namespace ComponentView {
	// lets try with only this one
	typedef struct userInput_ : baseComponent {
	private:
		bool up_;
		bool down_;
		bool left_;
		bool right_;

	public:
		bool dirty_;
		userInput_() { up_ = false; down_ = false; left_ = false; right_ = false; dirty_ = false; networked = false;  CompId = ComponentID::input; }
		userInput_(bool net) { up_ = false; down_ = false; left_ = false; right_ = false; dirty_ = false; networked = net; CompId = ComponentID::input; }
		~userInput_() = default;
		bool up() { return up_; }
		bool down() { return down_; }
		bool left() { return left_; }
		bool right() { return right_; }
		bool dirty() { return dirty_; }

		void setUp(bool set) { up_ = set; ; dirty_ = true; }
		void setDown(bool set) { down_ = set; dirty_ = true; }
		void setLeft(bool set) { left_ = set; dirty_ = true; }
		void setRight(bool set) { right_ = set; dirty_ = true; }
		void clear();
		int size() { return sizeof(ComponentID::ComponentID) + 5*sizeof(bool); }


		virtual void write(ProtoMessaging::UpdateEntityMessage& message, networkID netid) override;
		virtual void lock() {  }; // no mutex yet, so doesnt do anything really
	//	virtual void unlock(std::map<networkID, std::list<baseComponent*>>& updateMap, entt::entity& entity) override;

	} userInput;


	typedef struct position_ : baseComponent {
	private:
		uint32_t prevx_;
		uint32_t prevy_;
		uint32_t curx_;
		uint32_t cury_;

	public:
		position_() { prevx_ = 0; prevy_ = 0; curx_ = 0; cury_ = 0; networked = false;  CompId = ComponentID::Position; }
		~position_() = default;
		position_(bool spawn, bool net) {
			// set position at random value
			srand(time(NULL));
			curx_ = rand() % WIDTH;
			cury_ = rand() % HEIGHT;
		//	prevx_ = 0;
		//	prevy_ = 0;
			prevx_ = curx_;
			prevy_ = cury_;
			CompId = ComponentID::Position;
			networked = true;
			//std::cout << "prevx = " << prevx_ << " prevy = " << prevy_ << " curx = " << curx_ << " cury = " << cury_ << std::endl;
		}
		
		uint32_t prevx() { return prevx_; }
		uint32_t prevy() { return prevy_; }
		uint32_t curx() { return curx_; }
		uint32_t cury() { return cury_; }

		void setPrevx(uint32_t set) { prevx_ = set; }
		void setPrevy(uint32_t set) { prevy_ = set; }
		void setCurx(uint32_t set) { curx_ = set; }
		void setCury(uint32_t set) { cury_ = set; }
		int size() { return sizeof(ComponentID::ComponentID) + sizeof(bool) + 4* sizeof(uint32_t); }

		virtual void write(ProtoMessaging::UpdateEntityMessage& message, networkID netid) override;
		virtual void lock() {  }; // no mutex yet, so doesnt do anything really
	//	virtual void unlock(std::map<networkID, std::list<baseComponent*>>& updateMap, entt::entity& entity) override;
	} position;


	typedef struct mapObject_ : baseComponent {
	private:
		char mapChar_;

	public:
		mapObject_() { mapChar_ = 'X'; networked = false; CompId = ComponentID::MapObject; }
		mapObject_(char newChar) { mapChar_ = newChar; networked = false;  CompId = ComponentID::MapObject; }
		mapObject_(char newChar, bool net) { mapChar_ = newChar; networked = net;  CompId = ComponentID::MapObject; }
		mapObject_(bool net) { mapChar_ = 'X'; networked = net;  CompId = ComponentID::MapObject; }
		~mapObject_() = default;

		char mapChar() { return mapChar_; }
		void setMapChar(char set) { mapChar_ = set; }
		virtual void write(ProtoMessaging::UpdateEntityMessage& message, networkID netid) override;
		virtual void lock() {  }; // no mutex yet, so doesnt do anything really
		int size() { return sizeof(ComponentID::ComponentID) + sizeof(bool) + sizeof(char); }
	//	virtual void unlock(std::map<networkID, std::list<baseComponent*>>& updateMap, entt::entity& entity) override;
	} mapObject;

	typedef struct score_ : baseComponent {
	private:
		uint32_t points_;

	public:
		score_() { points_ = 0; networked = false;  CompId = ComponentID::Score; }
		score_(bool net) { points_ = 0; networked = true;  CompId = ComponentID::Score; }
		~score_() = default;
		uint32_t points() { return points_; }
		void setPoints(uint32_t set) { points_ = set;}
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
		virtual void lock() {  }; // no mutex yet, so doesnt do anything really
		int size() { return sizeof(ComponentID::ComponentID) + sizeof(bool) + sizeof(uint32_t); }
	//	virtual void unlock(std::map<networkID, std::list<baseComponent*>>& updateMap, entt::entity& entity) override;
	} score;

	typedef struct clientName_ : baseComponent 
	{
	private:
		std::string name_;
	public:
		clientName_() { name_ = ""; networked = false;  CompId = ComponentID::ClientName; }
		clientName_(std::string name_) { name_ = name_; networked = false;  CompId = ComponentID::ClientName; }
		clientName_(std::string name, bool net) { name_ = name; networked = net;  CompId = ComponentID::ClientName; }
		~clientName_() = default;
		std::string name() { return name_; }
		void setName(std::string set) { name_ = set;}
		virtual void write(ProtoMessaging::UpdateEntityMessage& message, networkID netid) override;
		virtual void lock() { }; // no mutex yet, so doesnt do anything really
		virtual int size() { return sizeof(ComponentID::ComponentID) + sizeof(bool); }
	//	virtual void unlock(std::map<networkID, std::list<baseComponent*>>& updateMap, entt::entity& entity) override;
	} clientName;


	// Adding health to the character
	typedef struct health_ : baseComponent {
	private:
		int32_t hp_;
	public:
		int32_t hp() { return hp_; }
		void setHp(int32_t set) { hp_ = set;}
		health_() { hp_ = 100; networked = false;  CompId = ComponentID::Health; }
		health_(bool net) { hp_ = 100; networked = net;  CompId = ComponentID::Health; }
		~health_() = default;
		int size() { return sizeof(ComponentID::ComponentID) + sizeof(bool) + sizeof(int32_t); }

	} health;

	// TODO: implement getters and setters, havent done it yet since this might be phased out
	typedef struct damageDone_ : baseComponent {
		int32_t damage;
		damageDone_() { damage = 0; networked = false;  CompId = ComponentID::DamageDone; }
		damageDone_(int32_t dm) { damage = dm; networked = false;  CompId = ComponentID::DamageDone;}
		~damageDone_() = default;
		virtual void write(ProtoMessaging::UpdateEntityMessage& message, networkID netid) override;
		int size() { return sizeof(ComponentID::ComponentID) + sizeof(bool); }
		virtual void lock() {  }; // no mutex yet, so doesnt do anything really
	//	virtual void unlock(std::map<networkID, std::list<baseComponent*>>& updateMap, entt::entity& entity) override;
	} damageDone;

	// To signify if the entity is networked.
	// Could be filled with additional network important information, but for now only filled with a boolean
	typedef struct networked_ : baseComponent {
		//std::list<component&> components; // List of components that are networked		// TODO: may need to pair the component with a type
		//long clientID;
		//bool isNetwoked; // If the entity should be networked
		virtual void write(ProtoMessaging::UpdateEntityMessage& message, networkID netid) override;
		int size() { return sizeof(ComponentID::ComponentID) + sizeof(bool); }
		virtual void lock() { }; // no mutex yet, so doesnt do anything really
	//	virtual void unlock(std::map<networkID, std::list<baseComponent*>>& updateMap, entt::entity& entity) override;
	} networked;
}