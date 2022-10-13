#pragma once

#include <Core/Layer.h>
#include <Core/Math.h>

#include <memory>

namespace Galactose {
	class Camera;
}

namespace GalactoseEditor {
	class EditorLayer : public Galactose::Layer {
	public:
		EditorLayer();

		void onUpdate() override;
		void onEvent(const std::shared_ptr<Galactose::Event>& a_event) override;

	private:
		std::shared_ptr<Galactose::Camera> m_camera;
		Galactose::Vector3 m_position;
		Galactose::Vector3 m_direction;
		const Galactose::Vector3 m_up;
	};
}
