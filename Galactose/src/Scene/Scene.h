#pragma once

#include "Core/Uuid.h"
#include "Core/Global.h"
#include "Core/Time.h"

#include <entity/registry.hpp>

#include <set>

namespace Galactose {
	class Entity;
	class Camera;
	class Event;
	class Script;

	class Scene {
	public:
		Scene(const std::string& name = "");
		virtual ~Scene();

		GT_UNMOVABLE(Scene);
		GT_UNCOPYABLE(Scene);

		std::string name() const { return m_name; }
		std::vector<Entity*> rootEntities() const { return m_rootEntities; }

		void render(const Camera* camera);
		void render() { render(m_mainCamera); }

		void save(const std::string& filePath) const;
		bool load(const std::string& filePath);

		Entity* getEntity(const Uuid& a_uuid) const;
		Camera* mainCamera() const { return m_mainCamera; }
		void setMainCamera(Camera* camera);

		void processEvent(const std::shared_ptr<Event>& event);

		Time& time() { return m_time; }

	private:
		Entity* getEntity(const entt::entity id) const;
		std::vector<Entity*> getEntities(const std::vector<entt::entity>& ids) const;
		
		void registerEventScript(Script* script);

		entt::registry m_registry;
		std::string m_name;
		std::vector<Entity*> m_rootEntities;
		std::unordered_map<Uuid, Entity*> m_entityMap;
		Camera* m_mainCamera = nullptr;
		// TO DO: sort scripts
		std::set<Script*> m_eventScripts;
		Time m_time;

		friend class Entity;
		friend class Component;
	};
}
