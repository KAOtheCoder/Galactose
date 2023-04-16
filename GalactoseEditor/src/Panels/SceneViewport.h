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
		SceneViewport(const std::shared_ptr<EditorSceneData>& sceneData);

		Galactose::Camera* getCamera() const override;
		void onFocusOut() override { m_rotate = false; }
		void onEvent(const std::shared_ptr<Galactose::Event>& a_event);

	private:
		Galactose::Scene m_privateScene;
		Galactose::Entity* m_cameraEntity = nullptr;
		bool m_rotate = false;
		Galactose::Vector2 m_cursorPos;
	};
}
