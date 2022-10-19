#pragma once

#include <entity/registry.hpp>

namespace Galactose {
	class Scene;

	class Entity {
	public:
		Entity(Scene* scene);

		template <class C>
		bool hasComponent() const { return m_scene->m_registry.has<C>(m_id); }

		template <class C>
		std::shared_ptr<C> addComponent() {
			if (hasComponent<C>())
				return nullptr;

			auto component = std::make_shared<C>();
			return m_scene->m_registry.emplace<std::shared_ptr<C>>(m_id, component);
		}

	private:
		entt::entity m_id;
		Scene* m_scene;
	};
}
