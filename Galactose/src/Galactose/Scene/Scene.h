#pragma once

#include "Galactose/Core/Uuid.h"
#include "Galactose/Core/Global.h"
#include "Galactose/Core/Time.h"

#include <entity/registry.hpp>

#include <set>

namespace Galactose {
	class Entity;
	class Camera;
	class Event;
	class Script;
	class OutSerializer;

	class Scene {
	public:
		GT_API Scene(const std::string& name = "");
		GT_API virtual ~Scene();

		GT_UNMOVABLE(Scene);
		GT_UNCOPYABLE(Scene);

		std::string name() const { return m_name; }
		std::vector<Entity*> rootEntities() const { return m_rootEntities; }

		GT_API void render(const Camera* camera);
		void render() { render(m_mainCamera); }

		GT_API void clear();
		GT_API void save(std::ostream& stream) const;
		GT_API bool load(std::istream& stream);

		GT_API Entity* getEntity(const Uuid& a_uuid) const;
		Camera* mainCamera() const { return m_mainCamera; }
		void setMainCamera(Camera* camera);

		GT_API void processEvent(const std::shared_ptr<Event>& event);
		GT_API void updateScripts();

		Time& time() { return m_time; }

	private:
		Entity* getEntity(const entt::entity id) const;
		std::vector<Entity*> getEntities(const std::vector<entt::entity>& ids) const;
		
		GT_API void registerScript(Script* script);

		entt::registry m_registry;
		std::string m_name;
		std::vector<Entity*> m_rootEntities;
		std::unordered_map<Uuid, Entity*> m_entityMap;
		Camera* m_mainCamera = nullptr;
		// TO DO: sort scripts
		std::set<Script*> m_scripts;
		Time m_time;

		friend class Entity;
		friend class Component;
	};
}
