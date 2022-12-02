#include "Component.h"
#include "Scene/Scene.h"

namespace Galactose {
	Entity* Component::entity() const { return m_data.scene->getEntity(m_data.entityId); }
}
