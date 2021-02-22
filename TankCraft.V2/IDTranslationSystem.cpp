#include "IDTranslationSystem.h"
#include "Components.h"
#include "TanksScene.h"


namespace TranslationSystem {


    networkID IDTranslation::createMapping(entt::registry& m_reg, std::map<networkID, entt::entity>& netToEnttid, entt::entity entityId)
    {
        // Allocate a new id for our entity
        networkID id = allocateID(m_reg);

        // Insert the mapping into the map
        netToEnttid[id] = entityId;

        // return the networkId associated with the mapping 
        return id;
    }


    networkID IDTranslation::allocateID(entt::registry& m_reg)
    {
        networkID itr;
        // Find a free space in the freelist, and set it as taken
        auto view = m_reg.view<GameView::freelist>();
        for (auto entity : view) {
            auto flist = view.get<GameView::freelist>(entity);
            itr = flist.lastMapping;
            
            // Find the next free spot in the freelist
            while (flist.map[itr]) {
                if (itr = MAX_ENTITIES - 1) {
                    itr = 0;
                }
                else {
                    itr++;
                }
            }

            // Set the location as used
            flist.map[itr] = 1;
            return itr;
        }
    }

    void IDTranslation::freeID(entt::registry& m_reg, networkID id)
    {
        // Get the freelist
        auto view = m_reg.view<GameView::freelist>();
        for (auto entity : view) {
            auto flist = view.get<GameView::freelist>(entity);
            // Set entity to be freed
            flist.map[id] = 0;
        }
    }

}