#pragma once

#include <entity/registry.hpp>

namespace Galactose {
	class Entity;
	class Camera;

	class Scene {
	public:
		Scene(const std::string& name);

		std::vector<Entity*> rootEntites() const { return m_rootEntities; }

		void render(const Camera& camera);

		void save(const std::string& filePath) const;

	private:
		Entity* getEntity(const entt::entity id) const;
		std::vector<Entity*> getEntities(const std::vector<entt::entity>& ids) const;

		entt::registry m_registry;
		std::string m_name;
		std::vector<Entity*> m_rootEntities;

		friend class Entity;
		friend class Component;
		friend class SceneObject;
	};
}
