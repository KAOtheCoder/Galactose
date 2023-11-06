#include "SpriteRenderer.h"
#include "Galactose/Serialization/OutSerializer.h"
#include "Galactose/Serialization/NodeSerializer.h"
#include "Galactose/Renderer/Texture.h"

namespace Galactose {
	void SpriteRenderer::saveContent(OutSerializer& a_out) const {
		std::filesystem::path texture_path;
		std::filesystem::path* texture = nullptr;

		if (sprite.texture()) {
			texture_path = sprite.texture()->filePath();
			texture = &texture_path;
		}

		a_out << OutSerializer::Key << "texture" << OutSerializer::Value << texture
			<< OutSerializer::Key << "color" << OutSerializer::Value << sprite.color()
			<< OutSerializer::Key << "pivot" << OutSerializer::Value << sprite.pivot()
			<< OutSerializer::Key << "size" << OutSerializer::Value << sprite.size();
	}

	bool SpriteRenderer::loadContent(const NodeSerializer& a_node) {
		const auto& textureNode = a_node["texture"];
		sprite.setTexture(textureNode.isNull() ? std::shared_ptr<Texture>() : Texture::create(textureNode.as<std::string>()));
		sprite.setColor(a_node["color"].as(sprite.color()));
		sprite.setPivot(a_node["pivot"].as(sprite.pivot()));
		sprite.setSize(a_node["size"].as(sprite.size()));

		return true;
	}
}
