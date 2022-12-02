#include "Entity.h"
#include "Scene.h"
#include "Components/Transform.h"
#include "Core/Global.h"

namespace Galactose {
	Entity* Entity::createOrphan(Scene* a_scene, const std::string& a_name) {
		GT_ASSERT(a_scene, "Scene is null.");
		const auto id = a_scene->m_registry.create();

		auto& entity = a_scene->m_registry.emplace<Entity>(id, a_name);
		entity.m_data = { a_scene, id };

		auto& transform = a_scene->m_registry.emplace<Transform>(id);
		transform.m_data = { a_scene, id };

		return &entity;
	}

	Entity* Entity::create(Scene* a_scene, const std::string& a_name) {
		const auto entity = createOrphan(a_scene, a_name);
		a_scene->m_rootEntities.push_back(entity->m_data.entityId);

		return entity;
	}

	Entity* Entity::create(Entity* a_parent, const std::string& a_name) {
		GT_ASSERT(a_parent, std::string("Parent can't be null, use '") + GT_STRINGIFY(Entity::create(Scene*)) + "' to create root entity.");
		auto entity = createOrphan(a_parent->m_data.scene, a_name);
		entity->m_parent = a_parent->m_data.entityId;
		a_parent->m_children.push_back(entity->m_data.entityId);

		return entity;
	}

	Entity::Entity(const std::string& a_name) :
		m_name(a_name) 
	{}

	Transform* Entity::getTransform() const {
		return &(m_data.scene->m_registry.get<Transform>(m_data.entityId));
	}

	void Entity::setParent(Entity* a_parent) {
		if (a_parent == parent())
			return;

		GT_ASSERT(!a_parent || a_parent->m_data.entityId != m_data.entityId, "Entity can't be parent of itself.");
		GT_ASSERT(!a_parent || a_parent->m_data.scene == m_data.scene, "Moving entities across scenes is not allowed.");

		auto& siblings = m_parent == entt::null ? m_data.scene->m_rootEntities : m_data.scene->getEntity(m_parent)->m_children;
		const auto& iter = std::find(siblings.begin(), siblings.end(), m_data.entityId);
		GT_ASSERT(iter != siblings.end(), "Cannot find entity id in parent.");
		siblings.erase(iter);

		if (a_parent) {
			a_parent->m_children.push_back(m_data.entityId);
			m_parent = a_parent->m_data.entityId;
		}
		else {
			m_data.scene->m_rootEntities.push_back(m_data.entityId);
			m_parent = entt::null;
		}
	}

	std::vector<Entity*> Entity::getChildren() const {
		return m_data.scene->getEntities(m_children);
	}
}
