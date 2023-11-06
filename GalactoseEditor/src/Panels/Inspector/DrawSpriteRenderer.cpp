#include "Inspector.h"
#include "Widgets/PropertyTable.h"
#include "FileDialog.h"

#include <Galactose/Scene/Components/SpriteRenderer.h>
#include <Galactose/Renderer/Texture.h>

#include <imgui.h>

using namespace Galactose;

namespace GalactoseEditor {
	template <>
	void Inspector::drawComponentContent<SpriteRenderer>(SpriteRenderer* a_spriteRenderer) {
		auto& sprite = a_spriteRenderer->sprite;

		const auto& texture = sprite.texture();
		std::filesystem::path textureFilePath = texture ? texture->filePath() : "";
		if (PropertyTable::fileInput("Texture", textureFilePath, { { "Texture", "png" } }, "Default")) {
			if (textureFilePath.empty())
				sprite.setTexture(nullptr);
			else {
				sprite.setTexture(Texture::create(textureFilePath));
				sprite.autoResize();
			}
		}

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
