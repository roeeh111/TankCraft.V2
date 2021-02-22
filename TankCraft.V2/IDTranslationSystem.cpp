#include "IDTranslationSystem.h"

void TranslationSystem::IDTranslation::freeID(networkID id)
{
}

networkID TranslationSystem::IDTranslation::createMapping(entt::entity entityId)
{
    return networkID();
}

entt::entity TranslationSystem::IDTranslation::get(networkID netId)
{
    return entt::entity();
}

networkID TranslationSystem::IDTranslation::generateID(entt::entity entityId)
{
    return networkID();
}
