#pragma once

#include "Scene/Entity.h"

namespace Galactose {
	class Component : public SceneObject {
	public:
		Entity* entity() const;
	};
}
