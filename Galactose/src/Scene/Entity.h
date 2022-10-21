#pragma once

#include "Object.h"

namespace Galactose {
	class Scene;
	class Component;

	class Entity : public Object {
	public:
		static Entity* create(Scene* scene);
		static Entity* create(Entity* parent);

		Entity() = default;

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

		Entity* parent() const { return m_parent == entt::null ? nullptr : getEntity(m_parent); }
		std::vector<Entity*> getChildren() const;

	private:
		Entity* getEntity(const entt::entity id) const { return &(m_data.scene->m_registry.get<Entity>(id)); }

		entt::entity m_parent = entt::null;
		entt::entity m_firstChild = entt::null;
		entt::entity m_nextSibling = entt::null;
	};
}
