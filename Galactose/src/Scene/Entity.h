#pragma once

#include "Object.h"

namespace Galactose {
	class Scene;

	class Entity : public Object {
	public:
		static Entity* create(Scene* scene);

		Entity() = default;

		template <class C>
		bool hasComponent() const { return m_data.scene->m_registry.any_of<C>(m_data.entityId); }

		template <class C, typename... Args>
		C* addComponent(Args&&... args) {
			if (hasComponent<C>())
				return nullptr;

			auto& component = m_data.scene->m_registry.emplace<C>(m_data.entityId, std::forward<Args>(args)...);
			auto ptr = &component;
			static_cast<Object*>(ptr)->m_data = m_data;
			return ptr;
		}
	};
}
