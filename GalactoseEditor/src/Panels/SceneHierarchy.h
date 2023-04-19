#pragma once

#include "Panel.h"

#include <memory>

namespace Galactose {
	class Entity;
}

namespace GalactoseEditor {
	class EditorSceneData;

	class SceneHierarchy : public Panel {
	public:
		SceneHierarchy(const std::shared_ptr<EditorSceneData>& sceneData);

		void onUpdate() override;
		void onEvent(const std::shared_ptr<Galactose::Event>& a_event) override;

	private:
		void drawEntityNode(Galactose::Entity* entity);

		std::shared_ptr<EditorSceneData> m_sceneData;
	};
}
