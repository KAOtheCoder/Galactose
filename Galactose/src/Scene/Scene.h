#pragma once

#include <entity/registry.hpp>

namespace Galactose {
	class Entity;

	class Scene {
	public:
		Scene(const std::string& name);

		std::vector<Entity*> getRootEntites() const { return getEntities(m_rootEntities); }

	private:
		Entity* getEntity(const entt::entity id) const;
		std::vector<Entity*> getEntities(const std::vector<entt::entity>& ids) const;

		entt::registry m_registry;
		std::string m_name;
		std::vector<entt::entity> m_rootEntities;

		friend class Entity;
		friend class Component;
		friend class Object;
	};
}
