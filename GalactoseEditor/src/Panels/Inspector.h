#pragma once

#include "Panel.h"

#include <memory>

struct ImGuiInputTextCallbackData;

namespace Galactose {
	class Vector3;
}

namespace GalactoseEditor {
	class EditorSceneData;

	class Inspector : public Panel {
	public:
		Inspector(const std::shared_ptr<EditorSceneData>& sceneData);

		void onUpdate() override;

	private:
		void drawLabel(const char* label);
		bool dragVector3Axis(const int axis, float& value);
		bool dragVector(const char* label, const int axisCount, float* value);
		bool drawComponentHeader(const char* label);
		void drawTransform();
		void drawSpriteRenderer();

		std::shared_ptr<EditorSceneData> m_sceneData;
	};
}
