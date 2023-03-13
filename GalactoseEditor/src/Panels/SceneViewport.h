#pragma once

#include "Panel.h"

#include <Scene/Scene.h>

#include <memory>

namespace Galactose {
	class Entity;
	class Framebuffer;
	class Event;
}

namespace GalactoseEditor {
	class EditorSceneData;

	class SceneViewport : public Panel {
	public:
		SceneViewport(const std::shared_ptr<EditorSceneData>& sceneData);

		void onUpdate() override;
		void onFocusOut() override { m_rotate = false; }

		void onEvent(const std::shared_ptr<Galactose::Event>& a_event);

	private:
		std::shared_ptr<EditorSceneData> m_sceneData;
		std::shared_ptr<Galactose::Framebuffer> m_framebuffer;
		Galactose::Scene m_privateScene;
		Galactose::Entity* m_cameraEntity = nullptr;
		bool m_rotate = false;
		Galactose::Vector2 m_cursorPos;
	};
}
