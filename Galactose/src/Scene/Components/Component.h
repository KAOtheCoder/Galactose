#pragma once

#include "Scene/Entity.h"

namespace Galactose {
	class Component : public Object {
	public:
		Entity* entity() const;
	};
}
