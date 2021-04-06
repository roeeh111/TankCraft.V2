#include "RegWrappers.h"

entt::entity RegWrapper::createEntity(entt::registry& m_reg, bool networked) {
	auto ret = m_reg.create();

	// If we want to network the entity, add a network component
	// If were a client, call addentity for the network system (figure out how we want to make sure the game doesnt run without that entity)
	if (networked) {
	//	m_reg.emplace<ComponentView::networked>(ret);			// TODO:	DO I NEED THIS??? RIGHT NOW IM NETWORKING COMPONENTS, NOT ENTITIES
		//											NOTE!!!! :: for now, add the entity to the network system

	}
	return ret;
}


// When should the client call add entity?
// should it call createentity, then tell the server?
// or, should it call addentity, and call createentity when it prompts us