#pragma once

#include "Component.h"
#include "Renderer/Sprite.h"

namespace Galactose {
	class SpriteRenderer : public Component {
	public:
		SpriteRenderer() = default;
		
		Sprite sprite;
	};
}