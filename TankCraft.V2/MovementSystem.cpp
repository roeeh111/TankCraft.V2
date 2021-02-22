#include "Components.h"
#include "MovementSystem.h"

namespace MovementSystem {
	void MovementSystem::updateMovement(entt::registry& m_reg, std::vector<std::vector<char>>& map)
	{
		// get all elements that have a mapObject and position, and then do the updates
		auto view = m_reg.view<GameView::mapObject, GameView::position>();
		for (auto entity : view) {

			auto& pos = view.get<GameView::position>(entity);
			//auto& scr = view.get<score>(entity);
			auto& disp = view.get<GameView::mapObject>(entity);

			if (pos.dirty) {

				// If the dirty bit is set, set old location to '.', and 
				// set new location to what it needs to be. also set dirty bit to 0

				// If weve met a cookie, eat it, add it to points, and remove it from the map
				if (map[pos.cury][pos.curx] == 'c') {
					// if we have a points component, increment it
					if (m_reg.has<GameView::score>(entity)) {
						// Get the entities score
						auto& scr = m_reg.get<GameView::score>(entity);
						scr.points++;
					}
				}
				map[pos.prevy][pos.prevx] = '.';
				map[pos.cury][pos.curx] = disp.mapChar;

				pos.dirty = 0;

			}
		}
	}
}
