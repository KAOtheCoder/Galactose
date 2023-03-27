#pragma once

#include "Core/Uuid.h"

#include <entity/registry.hpp>

namespace Galactose {
	class Entity;
	class Camera;

	class Scene {
	public:
		Scene(const std::string& name = "");
		virtual ~Scene();

		std::string name() const { return m_name; }
		std::vector<Entity*> rootEntities() const { return m_rootEntities; }

		void render(const Camera* camera);
		void render() { render(m_mainCamera); }

		void save(const std::string& filePath) const;
		bool load(const std::string& filePath);

		Entity* getEntity(const Uuid& a_uuid) const;
		Camera* mainCamera() const { return m_mainCamera; }
		void setMainCamera(Camera* camera);

	private:
		Entity* getEntity(const entt::entity id) const;
		std::vector<Entity*> getEntities(const std::vector<entt::entity>& ids) const;

		entt::registry m_registry;
		std::string m_name;
		std::vector<Entity*> m_rootEntities;
		std::unordered_map<Uuid, Entity*> m_entityMap;
		Camera* m_mainCamera = nullptr;

		friend class Entity;
		friend class Component;
	};
}
