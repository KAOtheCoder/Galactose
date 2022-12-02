#pragma once

#include "Widget.h"

namespace Galactose {
	class Entity;
}

namespace GalactoseEditor {
	class EditorSceneData;

	class SceneHierarchy : public Widget {
	public:
		SceneHierarchy(const std::shared_ptr<EditorSceneData>& sceneData);

		void onUpdate() override;

	private:
		void drawEntityNode(Galactose::Entity* entity);

		std::shared_ptr<EditorSceneData> m_sceneData;
	};
}
