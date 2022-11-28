#pragma once

#include "Object.h"

namespace Galactose {
	class Scene;
	class Component;
	class Transform;

	class Entity : public Object {
	public:
		static Entity* create(Scene* scene, const std::string& name = "");
		static Entity* create(Entity* parent, const std::string& name = "");

		Entity(const std::string& name = "");

		std::string name() const { return m_name; }

		Entity* parent() const { return m_parent == entt::null ? nullptr : m_data.scene->getEntity(m_parent); }
		void setParent(Entity* parent);

		std::vector<Entity*> getChildren() const;

		template <class C>
		bool hasComponent() const { 
			static_assert(std::is_base_of<Component, C>::value, "Type must inherit from Component.");
			return m_data.scene->m_registry.any_of<C>(m_data.entityId); 
		}

		template <class C, typename... Args>
		C* addComponent(Args&&... args) {
			static_assert(std::is_base_of<Component, C>::value, "Type must inherit from Component.");

			if (hasComponent<C>())
				return nullptr;

			auto& component = m_data.scene->m_registry.emplace<C>(m_data.entityId, std::forward<Args>(args)...);
			auto ptr = &component;
			static_cast<Object*>(ptr)->m_data = m_data;
			return ptr;
		}

		template <class C>
		C* getComponent() const {
			static_assert(std::is_base_of<Component, C>::value, "Type must inherit from Component.");

			return m_data.scene->m_registry.try_get<C>(m_data.entityId);
		}

		Transform* getTransform() const;

	private:
		static Entity* createOrphan(Scene* scene, const std::string& name = "");

		std::string m_name;
		entt::entity m_parent = entt::null;
		std::vector<entt::entity> m_children;
	};
}
