#pragma once

#include <Core/Layer.h>
#include <Core/Math.h>
#include <Renderer/Camera.h>
#include <Renderer/Texture.h>

namespace GalactoseEditor {
	class EditorLayer : public Galactose::Layer {
	public:
		EditorLayer();

		void onUpdate() override;
		void onEvent(const std::shared_ptr<Galactose::Event>& a_event) override;

	private:
		Galactose::Camera m_camera;
		std::shared_ptr<Galactose::Texture> m_texture;
		Galactose::Vector3 m_position;
		Galactose::Vector3 m_direction;
		const Galactose::Vector3 m_up;
		bool m_rotate = false;
		Galactose::Vector2 m_cursorPos;
	};
}
