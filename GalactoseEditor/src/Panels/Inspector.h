#pragma once

#include "Panel.h"

#include <memory>
#include <unordered_map>

#include <Math/Math.h>

struct ImGuiInputTextCallbackData;

namespace Galactose {
	class Texture;
}

namespace GalactoseEditor {
	class EditorSceneData;

	class Inspector : public Panel {
	public:
		Inspector(const std::shared_ptr<EditorSceneData>& sceneData);

		void onUpdate() override;

	private:
		static void drawLabel(const char* label);
		static bool dragVector3Axis(const int axis, float& value);
		static bool dragVector(const char* label, const int axisCount, float* value);
		static bool colorButton(const char* descId, Galactose::Vector4& color);
		bool drawFileInput(const char* label, std::string& path, const std::string& emptyText = "");
		bool iconButton(const char* icon);

		static bool drawComponentHeader(const char* label);

		void drawTransform();
		void drawSpriteRenderer();

		std::shared_ptr<EditorSceneData> m_sceneData;
		std::unordered_map<std::string, std::shared_ptr<Galactose::Texture>> m_icons;
	};
}
