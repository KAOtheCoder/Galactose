#pragma once

#include <entity/registry.hpp>

namespace Galactose {
	class Scene {
	public:
		Scene(const std::string& name);

	private:
		entt::registry m_registry;
		std::string m_name;

		friend class Entity;
		friend class Component;
		friend class Object;
	};
}
