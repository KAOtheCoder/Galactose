#include "Inspector.h"

#include <Scene/Components/SpriteRenderer.h>
#include <Renderer/Texture.h>

#include <imgui.h>

using namespace Galactose;

namespace GalactoseEditor {
	template <>
	void Inspector::drawComponentContent<SpriteRenderer>() {
		auto& sprite = getSelectedComponent<SpriteRenderer>()->sprite;

		const auto& texture = sprite.texture();
		std::string textureFilePath = texture ? texture->filePath() : "";
		if (drawFileInput("Texture", textureFilePath, "Default"))
			sprite.setTexture(textureFilePath.empty() ? nullptr : Texture::create(textureFilePath));

		auto color = sprite.color();
		if (colorButton("Color", color))
			sprite.setColor(color);

		auto size = sprite.size();
		if (dragVector("Size", 2, size.data()))
			sprite.setSize(size);

		auto pivot = sprite.pivot();
		if (dragVector("Pivot", 2, pivot.data()))
			sprite.setPivot(pivot);
	}
}
