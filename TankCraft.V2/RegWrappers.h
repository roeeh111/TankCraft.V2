#pragma once
#include <entt/entt.hpp>
#include "Components.h"

/*
* Wrapper of entity registry.create(), gives the option to label the entity as networked by adding a network component
*/

namespace RegWrapper{

	// Create an entity in the m_reg. If networked is set, add a network component to signify that it should be networked
	entt::entity createEntity(entt::registry& m_reg, bool networked);

	// Get component wrapper?

	// Set component wrappper, or just have each component have its own set method and have them implement them
	// (yeah tbh, lets have each component that is networked have to have setters that mutate the dirty bit)
}