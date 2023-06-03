#pragma once

#include "Panel.h"

#include <memory>

namespace Galactose {
	class Entity;
}

namespace GalactoseEditor {
	class EditorContext;

	class SceneHierarchy : public Panel {
	public:
		SceneHierarchy(const std::shared_ptr<EditorContext>& sceneContext);

		void onUpdate() override;
		void onEvent(const std::shared_ptr<Galactose::Event>& a_event) override;

	private:
		void drawEntityNode(Galactose::Entity* entity);

		std::shared_ptr<EditorContext> m_sceneContext;
	};
}
