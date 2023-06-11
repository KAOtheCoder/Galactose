#include "SpriteRenderer.h"
#include "Galactose/Scene/Serialize.h"
#include "Galactose/Renderer/Texture.h"

namespace Galactose {
	void SpriteRenderer::saveContent(YAML::Emitter& a_out) const {
		const auto& texture = sprite.texture()
			? YAML::convert<std::string>::encode(sprite.texture()->filePath()) 
			: YAML::Node(YAML::NodeType::Null);

		a_out << YAML::Key << "texture" << YAML::Value << texture
			<< YAML::Key << "color" << YAML::Value << sprite.color()
			<< YAML::Key << "pivot" << YAML::Value << sprite.pivot()
			<< YAML::Key << "size" << YAML::Value << sprite.size();
	}

	bool SpriteRenderer::loadContent(const YAML::Node& a_node) {
		const auto& textureNode = a_node["texture"];
		sprite.setTexture(textureNode.IsNull() ? std::shared_ptr<Texture>() : Texture::create(textureNode.as<std::string>()));
		sprite.setColor(a_node["color"].as<Vector4>(sprite.color()));
		sprite.setPivot(a_node["pivot"].as<Vector2>(sprite.pivot()));
		sprite.setSize(a_node["size"].as<Vector2>(sprite.size()));

		return true;
	}
}
