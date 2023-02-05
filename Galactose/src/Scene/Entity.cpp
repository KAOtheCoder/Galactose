#include "Entity.h"
#include "Scene.h"
#include "Components/Transform.h"
#include "Core/Global.h"

#include <yaml-cpp/yaml.h>

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

	void Entity::save(YAML::Emitter& a_emitter) const {
		a_emitter << YAML::BeginMap 
			<< YAML::Key << GT_STRINGIFY(Entity) << YAML::Value << YAML::BeginMap
			<< YAML::Key << "name" << YAML::Value << m_name
			<< YAML::Key << "components" << YAML::Value << YAML::BeginSeq;

		for (const auto component : getComponents())
			component->save(a_emitter);

		a_emitter << YAML::EndSeq << YAML::EndMap << YAML::EndMap;
	}

	Component* Entity::getComponent(const entt::id_type id) const {
		const auto pool = m_data.scene->m_registry.storage(id);

		if (pool && pool->contains(m_data.entityId))
			return static_cast<Component*>(pool->get(m_data.entityId));

		return nullptr;
	}

	std::vector<Component*> Entity::getComponents() const {
		std::vector<Component*> components;
		components.reserve(m_components.size() + 1); // +1 for Transform
		components.push_back(getTransform());

		for (const auto componentId : m_components)
			components.push_back(getComponent(componentId));

		return components;
	}
}
