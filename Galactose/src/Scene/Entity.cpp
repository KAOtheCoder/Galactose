#include "Entity.h"
#include "Scene.h"
#include "Components/Transform.h"
#include "Core/Global.h"
#include "Scene/Serialize.h"
#include <yaml-cpp/yaml.h>

namespace Galactose {
	Entity* Entity::createOrphan(Scene* a_scene, const std::string& a_name, const Uuid& a_id) {
		GT_ASSERT(a_scene, "Scene is null.");
		const auto entityId = a_scene->m_registry.create();

		auto& entity = a_scene->m_registry.emplace<Entity>(entityId, a_name, a_id);
		entity.m_scene = a_scene;
		entity.m_entityId = entityId;

		entity.addComponent<Transform>();

		return &entity;
	}

	Entity* Entity::create(Scene* a_scene, const std::string& a_name, const Uuid& a_id) {
		const auto entity = createOrphan(a_scene, a_name, a_id);
		a_scene->m_rootEntities.push_back(entity);

		return entity;
	}

	Entity* Entity::create(Entity* a_parent, const std::string& a_name, const Uuid& a_id) {
		GT_ASSERT(a_parent, std::string("Parent can't be null, use '") + GT_STRINGIFY(Entity::create(Scene*)) + "' to create root entity.");
		auto entity = createOrphan(a_parent->m_scene, a_name, a_id);
		entity->m_parent = a_parent;
		a_parent->m_children.push_back(entity);

		return entity;
	}

	Entity::Entity(const std::string& a_name, const Uuid& a_id) :
		m_id(a_id),
		m_name(a_name) 
	{}

	Transform* Entity::getTransform() const {
		return &(m_scene->m_registry.get<Transform>(m_entityId));
	}

	void Entity::setParent(Entity* a_parent) {
		if (a_parent == parent())
			return;

		GT_ASSERT(!a_parent || a_parent->m_entityId != m_entityId, "Entity can't be parent of itself.");
		GT_ASSERT(!a_parent || a_parent->m_scene == m_scene, "Moving entities across scenes is not allowed.");

		auto& siblings = m_parent ? m_parent->m_children : m_scene->m_rootEntities;
		const auto& iter = std::find(siblings.begin(), siblings.end(), this);
		GT_ASSERT(iter != siblings.end(), "Cannot find entity id in parent.");
		siblings.erase(iter);

		if (a_parent) {
			a_parent->m_children.push_back(this);
			m_parent = a_parent;
		}
		else {
			m_scene->m_rootEntities.push_back(this);
			m_parent = nullptr;
		}
	}

	void Entity::save(YAML::Emitter& a_emitter) const {
		a_emitter << YAML::BeginMap 
			<< YAML::Key << GT_STRINGIFY(Entity) << YAML::Value << YAML::BeginMap
			<< YAML::Key << "id" << YAML::Value << m_id
			<< YAML::Key << "name" << YAML::Value << m_name
			<< YAML::Key << "components" << YAML::Value << YAML::BeginSeq;

		for (const auto component : getComponents())
			component->save(a_emitter);

		a_emitter << YAML::EndSeq << YAML::EndMap << YAML::EndMap;
	}

	Component* Entity::getComponent(const entt::id_type id) const {
		const auto pool = m_scene->m_registry.storage(id);

		if (pool && pool->contains(m_entityId))
			return static_cast<Component*>(pool->get(m_entityId));

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
