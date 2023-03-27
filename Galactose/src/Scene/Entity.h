#pragma once

#include "SceneObject.h"
#include "Core/Uuid.h"
#include "Core/Global.h"

namespace YAML {
	class Emitter;
	class Node;
}

namespace Galactose {
	class Scene;
	class Component;
	class Transform;

	class Entity final : public SceneObject {
	public:
		static Entity* create(Scene* scene, const Uuid& id = Uuid::create());
		static Entity* create(Entity* parent, const Uuid& id = Uuid::create());

		GT_UNMOVABLE(Entity);

		// TO DO: move to private and make entt friend
		Entity(const Uuid& id = Uuid::create());
		~Entity() override;

		void destroy();

		Uuid uuid() const { return m_uuid; }

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

			auto component = &(m_scene->m_registry.emplace<C>(m_entityId, std::forward<Args>(args)...));
			static_cast<Component*>(component)->m_entity = this;
			m_components.push_back(component);
			component->start();

			return component;
		}

		template <class C>
		C* getComponent() const {
			static_assert(std::is_base_of<Component, C>::value, "Type must inherit from Component.");

			return m_scene->m_registry.try_get<C>(m_entityId);
		}

		template <class C>
		bool removeComponent() {
			if (m_scene->m_registry.remove<C>(m_entityId) > 0) {
				const int componentIndex = findComponent(entt::type_hash<C>::value());
				GT_ASSERT(componentIndex >= 0, "Component not found.");
				m_components.erase(m_components.begin() + componentIndex);

				return true;
			}

			return false;
		}

		Transform* getTransform() const;

		const std::vector<Component*>& components() const { return m_components; }

		void save(YAML::Emitter& out) const;
		bool load(const YAML::Node& node);

	private:
		static Entity* createOrphan(Scene* scene, const Uuid& id = Uuid::create());

		Component* addComponent(const std::string& name);
		Component* getComponent(const entt::id_type id) const;
		int findComponent(const entt::id_type id) const;
		void removeFromSiblings() const;

		Scene* m_scene = nullptr;
		entt::entity m_entityId = entt::null;
		Uuid m_uuid;
		std::string m_name;
		Entity* m_parent = nullptr;
		std::vector<Entity*> m_children;
		std::vector<Component*> m_components;
	};
}
