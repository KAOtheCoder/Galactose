#include "SpriteRenderer.h"
#include "Galactose/Scene/Serialize.h"
#include "Galactose/Scene/OutSerializer.h"
#include "Galactose/Renderer/Texture.h"

namespace Galactose {
	void SpriteRenderer::saveContent(OutSerializer& a_out) const {
		std::string texture_path;
		std::string* texture = nullptr;

		if (sprite.texture()) {
			texture_path = sprite.texture()->filePath();
			texture = &texture_path;
		}

		a_out << OutSerializer::Key << "texture" << OutSerializer::Value << texture
			<< OutSerializer::Key << "color" << OutSerializer::Value << sprite.color()
			<< OutSerializer::Key << "pivot" << OutSerializer::Value << sprite.pivot()
			<< OutSerializer::Key << "size" << OutSerializer::Value << sprite.size();
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
