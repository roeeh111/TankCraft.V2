#include "ScoreSystem.h"
#include "NetworkUtilitySystem.h"

namespace ScoreSystem {


	// TODO: update this so its dependant on an actual entity, not a map element
	// Loop through all coin entites and get the one in our position
	void updateScore(GameData::GameData& data, entt::entity entity, ComponentView::position& pos)
	{
		if (data.map[pos.cury()][pos.curx()] == 'C') {
			if (data.m_reg.has<ComponentView::score>(entity)) {
				// Get the entities score
				auto& scr = data.m_reg.get<ComponentView::score>(entity);
				scr.lock();

				// Get the coin entity that is at our location
				auto view = data.m_reg.view<ComponentView::pointsGiven, ComponentView::mapObject, ComponentView::position>();
				for (auto coinEntity : view) {
					auto& coinPos = view.get<ComponentView::position>(coinEntity);

					// if were in the same place
					if (pos.curx() == coinPos.curx() && pos.cury() == coinPos.cury()) {
						// Decrement the players health by the damage caused by the spikes
						scr.setPoints(scr.points() + view.get<ComponentView::pointsGiven>(coinEntity).getPoints());
						scr.unlock(data, entity);

						NetworkUtilitySystem::removeEntity(data, nullptr, TranslationSystem::getNetId(data, coinEntity), false, false);
						return;
					}
				}

				scr.unlock(data, entity);
			}
		}
	}



	void printScore(GameData::GameData& data)
	{
		// Print out points and client names of any entities that have those
		auto view = data.m_reg.view<ComponentView::score, ComponentView::clientName>();
		for (auto entity : view) {
			std::cout << "Points for client " << view.get<ComponentView::clientName>(entity).name() << ": " << view.get<ComponentView::score>(entity).points() << std::endl;
		}

	}
}