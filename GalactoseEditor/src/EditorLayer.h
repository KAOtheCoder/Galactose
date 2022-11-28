#pragma once

#include "Widgets/SceneHierarchy.h"

#include <Core/Layer.h>
#include <Math/Math.h>
#include <Renderer/Camera.h>
#include <Renderer/Texture.h>

namespace Galactose {
	class Window;
	class Scene;
	class Framebuffer;
}

namespace GalactoseEditor {
	class EditorLayer : public Galactose::Layer {
	public:
		EditorLayer(Galactose::Window* window);
		~EditorLayer() override;

		void onUpdate() override;
		void onEvent(const std::shared_ptr<Galactose::Event>& a_event) override;

	private:
		std::shared_ptr<Galactose::Framebuffer> m_framebuffer;
		std::shared_ptr<Galactose::Scene> m_scene;
		Galactose::Camera m_camera;
		std::shared_ptr<Galactose::Texture> m_texture;
		Galactose::Vector3 m_position;
		Galactose::Vector3 m_direction;
		const Galactose::Vector3 m_up;
		bool m_rotate = false;
		Galactose::Vector2 m_cursorPos;
		SceneHierarchy m_sceneHierarchy;
	};
}
