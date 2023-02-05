#pragma once

#include "Scene/Entity.h"

namespace YAML {
	class Emitter;
}

namespace Galactose {
	class Component : public SceneObject {
	public:
		Entity* entity() const;

		virtual void save(YAML::Emitter& emitter) const = 0;
	};
}
