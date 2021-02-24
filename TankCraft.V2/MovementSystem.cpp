#include "Components.h"
#include "MovementSystem.h"

namespace MovementSystem {
	void MovementSystem::updateMovement(SceneData::SceneData& data)
	{
		// get all elements that have a mapObject and position, and then do the updates
		auto view = data.m_reg.view<ComponentView::mapObject, ComponentView::position>();
		for (auto entity : view) {

			auto& pos = view.get<ComponentView::position>(entity);
			//auto& scr = view.get<score>(entity);
			auto& disp = view.get<ComponentView::mapObject>(entity);

			if (pos.dirty) {

				// If the dirty bit is set, set old location to '.', and 
				// set new location to what it needs to be. also set dirty bit to 0

				// If weve met a cookie, eat it, add it to points, and remove it from the map
				if (data.map[pos.cury][pos.curx] == 'c') {
					// if we have a points component, increment it
					if (data.m_reg.has<ComponentView::score>(entity)) {
						// Get the entities score
						auto& scr = data.m_reg.get<ComponentView::score>(entity);
						scr.points++;
					}
				}
				data.map[pos.prevy][pos.prevx] = '.';
				data.map[pos.cury][pos.curx] = disp.mapChar;

				pos.dirty = 0;

			}
		}
	}
}
