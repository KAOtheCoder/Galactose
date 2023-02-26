#pragma once

#include "Component.h"
#include "Renderer/Sprite.h"

namespace Galactose {
	class SpriteRenderer : public Component {
		GT_COMPONENT(SpriteRenderer)

	public:
		SpriteRenderer() = default;
		
		Sprite sprite;

	protected:
		void saveContent(YAML::Emitter& emitter) const override;
		bool loadContent(const YAML::Node& node) override;
	};
}
