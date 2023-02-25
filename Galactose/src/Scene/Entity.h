#pragma once

#include "SceneObject.h"
#include "Core/Uuid.h"

namespace YAML {
	class Emitter;
}

namespace Galactose {
	class Scene;
	class Component;
	class Transform;

	class Entity : public SceneObject {
	public:
		static Entity* create(Scene* scene, const std::string& name = "", const Uuid& id = Uuid::create());
		static Entity* create(Entity* parent, const std::string& name = "", const Uuid& id = Uuid::create());

		Entity(const std::string& name = "", const Uuid& id = Uuid::create());

		Scene* scene() const { return m_scene; }

		std::string name() const { return m_name; }
		void setName(const std::string& a_name) { m_name = a_name; }

		Entity* parent() const { return m_parent; }
		void setParent(Entity* parent);

		std::vector<Entity*> children() const { return m_children; }

		template <class C>
		bool hasComponent() const { 
			static_assert(std::is_base_of<Component, C>::value, "Type must inherit from Component.");
			return m_scene->m_registry.any_of<C>(m_entityId); 
		}

		template <class C, typename... Args>
		C* addComponent(Args&&... args) {
			static_assert(std::is_base_of<Component, C>::value, "Type must inherit from Component.");

			if (hasComponent<C>())
				return nullptr;

			auto& component = m_scene->m_registry.emplace<C>(m_entityId, std::forward<Args>(args)...);
			auto ptr = &component;
			static_cast<Component*>(ptr)->m_entity = this;

			if constexpr (!std::is_same_v<C, Transform>)
				m_components.push_back(entt::type_id<C>().hash());

			return ptr;
		}

		template <class C>
		C* getComponent() const {
			static_assert(std::is_base_of<Component, C>::value, "Type must inherit from Component.");

			return m_scene->m_registry.try_get<C>(m_entityId);
		}

		Transform* getTransform() const;

		std::vector<Component*> getComponents() const;

		void save(YAML::Emitter& emitter) const;

	private:
		static Entity* createOrphan(Scene* scene, const std::string& name = "", const Uuid& id = Uuid::create());

		Component* getComponent(const entt::id_type id) const;

		Scene* m_scene = nullptr;
		entt::entity m_entityId = entt::null;
		Uuid m_id;
		std::string m_name;
		Entity* m_parent = nullptr;
		std::vector<Entity*> m_children;
		std::vector<entt::id_type> m_components;
	};
}
