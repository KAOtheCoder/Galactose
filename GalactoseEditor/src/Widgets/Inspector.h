#pragma once

#include "Widget.h"

#include <memory>

struct ImGuiInputTextCallbackData;

namespace Galactose {
	class Vector3;
}

namespace GalactoseEditor {
	class EditorSceneData;

	class Inspector : public Widget {
	public:
		Inspector(const std::shared_ptr<EditorSceneData>& sceneData);

		void onUpdate() override;

	private:
		bool dragVector3Axis(const int axis, Galactose::Vector3& value);
		bool dragVector3(const char* label, Galactose::Vector3& value);
		void drawTransform();

		std::shared_ptr<EditorSceneData> m_sceneData;
	};
}
