#include "Scene.h"
#include "Entity.h"
#include "Components/Transform.h"
#include "Components/SpriteRenderer.h"
#include "Renderer/Renderer.h"

#include <yaml-cpp/yaml.h>

#include <fstream>

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

	void Scene::render(const Camera& a_camera) {
		auto renderer = Renderer::renderer();

		renderer->clear();
		renderer->setViewProjection(a_camera);

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
}
