#include "IDTranslationSystem.h"
#include "Components.h"
#include "CreateEntity.h"

namespace TranslationSystem {
    networkID IDTranslation::createMapping(SceneComponent::SceneComponent& data, entt::entity entityId)
    {
        // Allocate a new id for our entity
        networkID id = allocateID(data.m_reg);

        // Insert the mapping into the map
        data.netToEnttid[id] = entityId;

        // return the networkId associated with the mapping 
        return id;
    }

    networkID IDTranslation::setMapping(SceneComponent::SceneComponent& data, networkID netId, entt::entity entityId)
    {
        auto flist = getFreelist(data.m_reg);
        data.netToEnttid[netId] = entityId;
        return netId;
    }

    entt::entity IDTranslation::getEntity(SceneComponent::SceneComponent& data, networkID netId)
    {
        return data.netToEnttid[netId];
    }

    void IDTranslation::addEntity(SceneComponent::SceneComponent& data, ProtoMessaging::AddRemoveEntityMessage* msg)
    {
        // Assume the message has been verified, or add verification for udp

        // make a mapping of the given netid and a local entity netid value
        auto newEntity = RegWrapper::createEntity(data.m_reg, true);

        setMapping(data, msg->netid(), newEntity);  //	TODO: change this to createEntity

    }

    void IDTranslation::removeEntity(SceneComponent::SceneComponent& data, ProtoMessaging::AddRemoveEntityMessage* msg)
    {
        freeID(data, msg->netid());
    }





    networkID IDTranslation::allocateID(entt::registry& m_reg)
    {
        networkID itr;
        // Find a free space in the freelist, and set it as taken
        auto &flist = getFreelist(m_reg);

        // set our current iterator to the last mapping + 1
        itr = flist.lastMapping;
            
        // Find the next free spot in the freelist
        while (flist.map[itr]) {
            if (itr == (MAX_ENTITIES - 1)) {
                itr = 0;
            }
            else {
                itr++;
            }
        }

        // Set the location as used
        flist.map[itr] = 1;
        flist.lastMapping = itr + 1;
        return itr;
    }

    FreeListComponent::freelist &IDTranslation::getFreelist(entt::registry& m_reg)
    {
        auto view = m_reg.view<FreeListComponent::freelist>();
        for (auto entity : view) {
            FreeListComponent::freelist &flist = view.get<FreeListComponent::freelist>(entity);
            return flist;
        }
        std::cerr << "cant find a freelist" << std::endl;
    }


    void IDTranslation::freeID(SceneComponent::SceneComponent& data, networkID id)
    {
        // Get the freelist
        auto flist = getFreelist(data.m_reg);
        flist.map[id] = 0;
    }



}