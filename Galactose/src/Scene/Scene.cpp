#include "Scene.h"
#include "Entity.h"
#include "Components/Transform.h"
#include "Components/SpriteRenderer.h"
#include "Components/Camera.h"
#include "Renderer/Renderer.h"
#include "Serialize.h"

#include <yaml-cpp/yaml.h>

#include <fstream>
#include <iostream>

namespace Galactose {
	Scene::Scene(const std::string& a_name) :
		m_name(a_name)
	{}

	Entity* Scene::getEntity(const entt::entity id) const {
		return const_cast<Entity*>(&(m_registry.get<Entity>(id)));
	}

	std::vector<Entity*> Scene::getEntities(const std::vector<entt::entity>& a_ids) const {
		const auto entites_size = a_ids.size();
		std::vector<Entity*> children(entites_size);

		for (size_t i = 0; i < entites_size; ++i)
			children[i] = getEntity(a_ids[i]);

		return children;
	}

	Entity* Scene::getEntity(const Uuid& a_uuid) const {
		const auto& iter = m_entityMap.find(a_uuid);
		return iter == m_entityMap.end() ? nullptr : iter->second;
	}

	void Scene::render(const Camera* a_camera) {
		auto renderer = Renderer::renderer();

		renderer->clear();
		renderer->setViewProjection(a_camera->viewProjectionMatrix());

		const auto& group = m_registry.group<Transform>(entt::get<SpriteRenderer>);

		for (const auto entityId : group) {
			const auto& [transform, spriteRenderer] = group.get<Transform, SpriteRenderer>(entityId);

			renderer->drawSprite(transform.localToWorldMatrix(), spriteRenderer.sprite);
		}
	}

	void Scene::save(const std::string& filePath) const {
		YAML::Emitter emitter;
		emitter << YAML::BeginMap
			<< YAML::Key << "name" << YAML::Value << m_name
			<< YAML::Key << "entities" << YAML::Value << YAML::BeginSeq;
		
		m_registry.each([&](const auto id) { getEntity(id)->save(emitter); });
		
		emitter << YAML::EndSeq << YAML::EndMap;

		std::ofstream fileStream(filePath);
		fileStream << emitter.c_str();
	}

	bool Scene::load(const std::string& filePath) {
		try
		{
			const auto& node = YAML::LoadFile(filePath);

			const auto& entitiesNode = node["entities"];

			for (const auto& entityWrapperNode : entitiesNode) {
				const auto& entityNode = entityWrapperNode["Entity"];
				const auto& uuid = entityNode["uuid"].as<Uuid>();
				Entity::create(this, uuid);
			}

			for (const auto& entityWrapperNode : entitiesNode) {
				const auto& entityNode = entityWrapperNode["Entity"];
				const auto& uuid = entityNode["uuid"].as<Uuid>();
				m_entityMap[uuid]->load(entityWrapperNode);
			}
		}
		catch (const YAML::Exception& x)
		{
			std::cerr << "Failed to load file '" << filePath << "': " << x.what() << std::endl;
			return false;
		}

		return true;
	}
}
