#include "SpriteRenderer.h"
#include "Scene/Serialize.h"
#include "Core/Global.h"
#include "Renderer/Texture.h"

namespace Galactose {
	void SpriteRenderer::save(YAML::Emitter& a_out) const {
		const auto& texture = sprite.texture()
			? YAML::convert<std::string>::encode(sprite.texture()->filePath()) 
			: YAML::Node(YAML::NodeType::Null);

		beginSave(a_out, GT_STRINGIFY(SpriteRenderer));

		a_out << YAML::Key << "texture" << YAML::Value << texture
			<< YAML::Key << "color" << YAML::Value << sprite.color()
			<< YAML::Key << "pivot" << YAML::Value << sprite.pivot()
			<< YAML::Key << "size" << YAML::Value << sprite.size();

		endSave(a_out);
	}
}
