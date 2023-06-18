#pragma once

#include "Component.h"
#include "Galactose/Renderer/Sprite.h"

namespace Galactose {
	class SpriteRenderer final : public Component {
		GT_COMPONENT(SpriteRenderer)

	public:
		SpriteRenderer() = default;
		
		Sprite sprite;

	protected:
		GT_API void saveContent(OutSerializer& emitter) const override;
		GT_API bool loadContent(const NodeSerializer& node) override;
	};
}
