#include "SpriteRenderer.h"
#include "Scene/Serialize.h"
#include "Renderer/Texture.h"

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
}
