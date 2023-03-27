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

	Scene::~Scene() {
		while (!m_rootEntities.empty())
			m_rootEntities.back()->destroy();
	}

	std::vector<Entity*> Scene::getEntities(const std::vector<entt::entity>& a_ids) const {
		const auto entites_size = a_ids.size();
		std::vector<Entity*> entities(entites_size);

		for (size_t i = 0; i < entites_size; ++i)
			entities[i] = getEntity(a_ids[i]);

		return entities;
	}

	Entity* Scene::getEntity(const Uuid& a_uuid) const {
		const auto& iter = m_entityMap.find(a_uuid);
		return iter == m_entityMap.end() ? nullptr : iter->second;
	}

	void Scene::setMainCamera(Camera* a_camera) {
		if (!a_camera) {
			const auto& view = m_registry.view<Camera>();
			for (const auto entity : view) {
				m_mainCamera = &(view.get<Camera>(entity));
				return;
			}
		}

		m_mainCamera = a_camera;
	}

	void Scene::render(const Camera* a_camera) {
		auto renderer = Renderer::renderer();

		renderer->clear();

		if (!a_camera)
			return;

		renderer->setViewProjection(a_camera->viewProjectionMatrix());

		const auto& view = m_registry.view<SpriteRenderer>();

		for (const auto entity : view) {
			auto& spriteRenderer = view.get<SpriteRenderer>(entity);
			renderer->drawSprite(spriteRenderer.getTransform()->localToWorldMatrix(), spriteRenderer.sprite);
		}
	}

	void Scene::save(const std::string& a_filePath) const {
		YAML::Emitter emitter;
		emitter << YAML::BeginMap
			<< YAML::Key << "name" << YAML::Value << m_name
			<< YAML::Key << "entities" << YAML::Value << YAML::BeginSeq;
		
		m_registry.each([&](const auto a_id) { getEntity(a_id)->save(emitter); });
		
		emitter << YAML::EndSeq << YAML::EndMap;

		std::ofstream fileStream(a_filePath);
		fileStream << emitter.c_str();
	}

	bool Scene::load(const std::string& a_filePath) {
		try
		{
			const auto& node = YAML::LoadFile(a_filePath);

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
			std::cerr << "Failed to load file '" << a_filePath << "': " << x.what() << std::endl;
			return false;
		}

		return true;
	}
}
