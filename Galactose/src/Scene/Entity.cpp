#include "GalactosePCH.h"
#include "Entity.h"
#include "Scene.h"

namespace Galactose {
	Entity* Entity::create(Scene* a_scene, const std::string& a_name) {
		GT_ASSERT(a_scene, "Scene is null.");
		const auto id = a_scene->m_registry.create();
		auto& entity = a_scene->m_registry.emplace<Entity>(id, a_name);
		entity.m_data = { a_scene, id };

		return &entity;
	}

	Entity* Entity::create(Entity* a_parent, const std::string& a_name) {
		GT_ASSERT(a_parent, std::string("Parent can't be null, use '") + GT_STRINGIFY(Entity::create(Scene*)) + "' instead.");
		auto entity = create(a_parent->m_data.scene, a_name);
		entity->m_parent = a_parent->m_data.entityId;
		
		if (a_parent->m_firstChild == entt::null) {
			a_parent->m_firstChild = entity->m_data.entityId;
		}
		else {
			Entity* child = a_parent->getEntity(a_parent->m_firstChild);

			while (child->m_nextSibling != entt::null)
				child = a_parent->getEntity(child->m_nextSibling);

			child->m_nextSibling = entity->m_data.entityId;
		}

		return entity;
	}

	Entity::Entity(const std::string& a_name) :
		m_name(a_name) 
	{}

	std::vector<Entity*> Entity::getChildren() const {
		std::vector<Entity*> children;

		if (m_firstChild != entt::null) {
			Entity* child = getEntity(m_firstChild);
			children.push_back(child);

			while (child->m_nextSibling != entt::null) {
				child = getEntity(child->m_nextSibling);
				children.push_back(child);
			}
		}

		return children;
	}
}
