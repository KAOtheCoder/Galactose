#include "SpriteRenderer.h"
#include "Math/MathStream.h"
#include "Core/Global.h"
#include "Renderer/Texture.h"

namespace Galactose {
	void SpriteRenderer::save(YAML::Emitter& a_emitter) const {
		const auto& texture = sprite.texture()
			? YAML::convert<std::string>::encode(sprite.texture()->filePath()) 
			: YAML::Node(YAML::NodeType::Null);

		a_emitter << YAML::BeginMap
			<< YAML::Key << GT_STRINGIFY(SpriteRenderer) << YAML::Value << YAML::BeginMap
			<< YAML::Key << "texture" << YAML::Value << texture
			<< YAML::Key << "color" << YAML::Value << sprite.color()
			<< YAML::Key << "pivot" << YAML::Value << sprite.pivot()
			<< YAML::Key << "size" << YAML::Value << sprite.size()
			<< YAML::EndMap << YAML::EndMap;
	}
}
