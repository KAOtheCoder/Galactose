#include "GalactosePCH.h"
#include "Component.h"
#include "Scene/Scene.h"

namespace Galactose {
	Entity* Component::entity() const { return &(m_data.scene->m_registry.get<Entity>(m_data.entityId)); }
}
