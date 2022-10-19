#pragma once

#include "GalactosePCH.h"
#include <entity/registry.hpp>

namespace Galactose {
	class Entity;

	class Scene {
	public:
		Scene(const std::string& name);

		std::shared_ptr<Entity> addEntity();

	private:
		entt::registry m_registry;
		std::string m_name;

		friend class Entity;
	};
}
