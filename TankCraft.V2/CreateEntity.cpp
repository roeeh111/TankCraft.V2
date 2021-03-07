
#include "CreateEntity.h"

entt::entity RegWrapper::createEntity(entt::registry& m_reg, bool networked) {
	auto ret = m_reg.create();

	// If we want to network the entity, add a network component
	if (networked) {
		m_reg.emplace<ComponentView::networked>(ret);
	}
	return ret;
}