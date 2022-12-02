#pragma once

#include "Widget.h"

#include <memory>

namespace GalactoseEditor {
	class EditorSceneData;

	class Inspector : public Widget {
	public:
		Inspector(const std::shared_ptr<EditorSceneData>& sceneData);

		void onUpdate() override;

	private:
		std::shared_ptr<EditorSceneData> m_sceneData;
	};
}
