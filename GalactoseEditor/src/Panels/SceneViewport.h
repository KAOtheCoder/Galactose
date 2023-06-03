#pragma once

#include "Viewport.h"

#include <Scene/Scene.h>

namespace Galactose {
	class Entity;
	class Event;
}

namespace GalactoseEditor {
	class SceneViewport : public Viewport {
	public:
		SceneViewport(const std::shared_ptr<EditorContext>& sceneContext);

		Galactose::Camera* getCamera() const override;
		void onEvent(const std::shared_ptr<Galactose::Event>& event) override;
		void onUpdate() override;

	private:
		Galactose::Scene m_privateScene;
		Galactose::Entity* m_cameraEntity = nullptr;
		bool m_usingManipulator = false;
	};
}
