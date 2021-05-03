#include "GameData.h"
#include "IDTranslationSystem.h"

void baseComponent_::unlock(GameData::GameData& data, const entt::entity& entity)
{
	//std::cout << "calling unlock" << std::endl;
	// TODO: actually unlock the component

	// If the component is networked, append the component to the map
	if (networked) {
		networkID id = TranslationSystem::getNetId(data, entity);
		// If the netid isnt in the map, allocate a new header for it
		if (data.compUpdateMap.count(id) == 0) {
			data.compUpdateMap[id] = UpdatePacketHeader::UpdatePacketHeader(id);
		}
		// Push back the component id
		data.compUpdateMap[id].ids.push_back(CompId);

		//if (data.updateMap.count(id) == 0) {
		//	std::cout << "\nallocating new map for id = " << id << std::endl;
		//	data.updateMap[id] = std::list<baseComponent*>();
		//}
		//data.updateMap[id].push_back(this);						// PROBLEM MIGHT BE HERE, WERE ADDING A POINTER TO THIS, AND THE POINTER COULD MOVE!!
	}
}


/*
1) Instead of doing a map with a bunch of base pointers, we could just add to a reflection update object by inserting a compid and netid into a header
	(keep a list of headers)

2) to flush, we loop through the header, and call serialize on the compid-netid pair
	(requires a switch statement of compid->serialize functions, but can be generated with code)

Pros:
	- quicker, since we dont need a layer between the map and the header process
	- simpler, no need to hold this update map
	- easier to debug? we dont need to track a pointer to an object that might move
Cons:
	- a little more work
	- may be slower


Changes Required:
1) in unlock, add the compid to a map of netid->list<compid> instead of a pointer (done)

2) changing the update map to netid->list<compid> (NOTE: may cause issues somewhere else in the code thats using it) (done)

3) writing a switch statement to convert between compid->serialize functions (note, wed need to call serialize(get<component>))

4) change everywhere in the currently used code to not use the update map
*/
