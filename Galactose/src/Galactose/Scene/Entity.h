#pragma once

#include "SceneObject.h"
#include "Galactose/Core/Uuid.h"
#include "Galactose/Core/Global.h"

namespace YAML {
	class Node;
}

namespace Galactose {
	class Scene;
	class Component;
	class Transform;
	class OutSerializer;

	class Entity final : public SceneObject {
	public:
		GT_API static Entity* create(Scene* scene, const Uuid& id = Uuid::create());
		static Entity* create(Entity* parent, const Uuid& id = Uuid::create());

		GT_UNMOVABLE(Entity);

		// TO DO: move to private and make entt friend
		Entity(const Uuid& id = Uuid::create());
		~Entity() override;

		GT_API void destroy();

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

		template <class C>
		C* addComponent() {
			static_assert(std::is_base_of<Component, C>::value, "Type must inherit from Component.");

			if (hasComponent<C>())
				return nullptr;

			auto component = &(m_scene->m_registry.emplace<C>(m_entityId));
			static_cast<Component*>(component)->m_entity = this;
			component->start();

			if constexpr (std::is_base_of_v<Script, C>) {
				m_scene->registerScript(static_cast<Script*>(component));
				m_components.push_back(component);
			}
			else {
				m_components.insert(m_components.begin() + m_scriptOffset, component);
				++m_scriptOffset;
			}

			return component;
		}

		template <class C>
		C* getComponent() const {
			static_assert(std::is_base_of_v<Component, C>, "Type must inherit from Component.");

			return m_scene->m_registry.try_get<C>(m_entityId);
		}

		template <class C>
		bool removeComponent() {
			if (m_scene->m_registry.remove<C>(m_entityId) > 0) {
				constexpr bool isScript = std::is_base_of_v<Script, C>;
				const int componentIndex = findComponent(entt::type_hash<C>::value(), isScript);
				GT_ASSERT(componentIndex >= 0, "Component not found.");
				m_components.erase(m_components.begin() + componentIndex);

				if constexpr (!isScript)
					--m_scriptOffset;

				return true;
			}

			return false;
		}

		GT_API Transform* getTransform() const;

		const std::vector<Component*>& components() const { return m_components; }
		size_t scriptOffset() const { return m_scriptOffset; }

		void save(OutSerializer& out) const;
		bool load(const YAML::Node& node);

	private:
		static Entity* createOrphan(Scene* scene, const Uuid& id = Uuid::create());

		Component* addComponent(const std::string& name);
		Component* getComponent(const uint32_t id) const;
		GT_API int findComponent(const uint32_t id, const bool script) const;
		void removeFromSiblings() const;

		Scene* m_scene = nullptr;
		entt::entity m_entityId = entt::null;
		Uuid m_uuid;
		std::string m_name;
		Entity* m_parent = nullptr;
		std::vector<Entity*> m_children;
		std::vector<Component*> m_components;
		size_t m_scriptOffset = 0;
	};
}
