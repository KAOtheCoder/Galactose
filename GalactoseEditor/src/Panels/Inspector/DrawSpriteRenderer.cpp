#include "Inspector.h"

#include <Scene/Components/SpriteRenderer.h>
#include <Renderer/Texture.h>

#include <imgui.h>

using namespace Galactose;

namespace GalactoseEditor {
	template <>
	void Inspector::drawComponent<SpriteRenderer>() {
		if (!drawComponentHeader("Sprite Renderer"))
			return;

		auto spriteRenderer = getSelectedComponent<SpriteRenderer>();
		auto& sprite = spriteRenderer->sprite;

		if (ImGui::BeginTable("SpriteRenderer", 2, ImGuiTableFlags_SizingStretchProp)) {
			const auto& texture = sprite.texture();
			auto textureFilePath = texture ? texture->filePath() : "";
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

			ImGui::EndTable();
		}
	}
}
