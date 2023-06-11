#include "Entity.h"
#include "Scene.h"
#include "Components/Transform.h"
#include "Galactose/Core/Global.h"
#include "Galactose/Scene/Serialize.h"

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

	Entity::~Entity() {
		removeFromSiblings();

		for (const auto component : m_components)
			m_scene->m_registry.storage(component->type())->erase(m_entityId);

		while (!m_children.empty())
			m_children.back()->destroy(); // Make sure all other components deleted first
	}

	void Entity::destroy() {
		m_scene->m_registry.erase<Entity>(m_entityId);
		m_scene->m_registry.destroy(m_entityId);
	}

	Transform* Entity::getTransform() const {
		return &(m_scene->m_registry.get<Transform>(m_entityId));
	}

	void Entity::removeFromSiblings() const {
		auto& siblings = m_parent ? m_parent->m_children : m_scene->m_rootEntities;
		const auto& iter = std::find(siblings.begin(), siblings.end(), this);
		GT_ASSERT(iter != siblings.end(), "Cannot find entity id in siblings.");
		siblings.erase(iter);
	}

	void Entity::setParent(Entity* a_parent) {
		if (a_parent == parent())
			return;

		GT_ASSERT(!a_parent || a_parent != this, "Entity can't be parent of itself.");
		GT_ASSERT(!a_parent || a_parent->m_scene == m_scene, "Moving entities across scenes is not allowed.");

		removeFromSiblings();

		if (a_parent)
			a_parent->m_children.push_back(this);
		else
			m_scene->m_rootEntities.push_back(this);
		
		m_parent = a_parent;
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
			auto component = getComponent(Component::MetaBase::meta(componentName)->type());

			if (!component)
				component = addComponent(componentName);

			component->load(componentWrapperNode);
		}

		return true;
	}

	Component* Entity::addComponent(const std::string& a_name) {
		return Component::MetaBase::meta(a_name)->create(this);
	}

	Component* Entity::getComponent(const uint32_t a_id) const {
		const auto pool = m_scene->m_registry.storage(a_id);
		if (pool && pool->contains(m_entityId))
			return static_cast<Component*>(m_scene->m_registry.storage(a_id)->get(m_entityId));

		return nullptr;
	}

	int Entity::findComponent(const uint32_t a_id, const bool a_script) const {
		const auto end = a_script ? m_components.size() : m_scriptOffset;
		for (int i = a_script ? (int)m_scriptOffset : 0; i < end; ++i)
			if (m_components[i]->type() == a_id)
				return i;
		
		return -1;
	}
}
