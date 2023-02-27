#include "Entity.h"
#include "Scene.h"
#include "Components/Transform.h"
#include "Core/Global.h"
#include "Scene/Serialize.h"
#include <yaml-cpp/yaml.h>

namespace Galactose {
	Entity* Entity::createOrphan(Scene* a_scene, const Uuid& a_uuid) {
		GT_ASSERT(a_scene, "Scene is null.");
		const auto entityId = a_scene->m_registry.create();

		auto entity = &(a_scene->m_registry.emplace<Entity>(entityId, a_uuid));
		entity->m_scene = a_scene;
		entity->m_entityId = entityId;
		a_scene->m_entityMap[a_uuid] = entity;

		entity->addComponent<Transform>();

		return entity;
	}

	Entity* Entity::create(Scene* a_scene, const Uuid& a_id) {
		const auto entity = createOrphan(a_scene, a_id);
		a_scene->m_rootEntities.push_back(entity);

		return entity;
	}

	Entity* Entity::create(Entity* a_parent, const Uuid& a_id) {
		GT_ASSERT(a_parent, std::string("Parent can't be null, use '") + GT_STRINGIFY(Entity::create(Scene*)) + "' to create root entity.");
		auto entity = createOrphan(a_parent->m_scene, a_id);
		entity->m_parent = a_parent;
		a_parent->m_children.push_back(entity);

		return entity;
	}

	Entity::Entity(const Uuid& a_id) :
		m_uuid(a_id)
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
		const auto& parentNode = m_parent
			? YAML::convert<Uuid>::encode(m_parent->m_uuid)
			: YAML::Node(YAML::NodeType::Null);

		a_emitter << YAML::BeginMap 
			<< YAML::Key << GT_STRINGIFY(Entity) << YAML::Value << YAML::BeginMap
			<< YAML::Key << "uuid" << YAML::Value << m_uuid
			<< YAML::Key << "parent" << YAML::Value << parentNode
			<< YAML::Key << "name" << YAML::Value << m_name
			<< YAML::Key << "components" << YAML::Value << YAML::BeginSeq;

		for (const auto component : components())
			component->save(a_emitter);

		a_emitter << YAML::EndSeq << YAML::EndMap << YAML::EndMap;
	}

	bool Entity::load(const YAML::Node& a_node) {
		const auto& entityNode = a_node["Entity"];
		m_name = entityNode["name"].as<std::string>();
		const auto& parentNode = entityNode["parent"];
		setParent(parentNode.IsNull() ? nullptr : m_scene->getEntity(parentNode.as<Uuid>()));

		for (const auto& componentWrapperNode : entityNode["components"]) {
			const auto& componentName = componentWrapperNode.begin()->first.as<std::string>();
			auto component = getComponent(Component::Meta::meta(componentName)->id);

			if (!component)
				component = addComponent(componentName);

			component->load(componentWrapperNode);
		}

		return true;
	}

	Component* Entity::addComponent(const std::string& a_name) {
		return Component::Meta::meta(a_name)->creator(this);
	}

	Component* Entity::getComponent(const entt::id_type a_id) const {
		const auto pool = m_scene->m_registry.storage(a_id);
		if (pool && pool->contains(m_entityId))
			return static_cast<Component*>(m_scene->m_registry.storage(a_id)->get(m_entityId));

		return nullptr;
	}
}
