#pragma once

#include <Core/Layer.h>

#include <memory>

namespace Galactose {
	class Camera;
}

namespace GalactoseEditor {
	class EditorLayer : public Galactose::Layer {
	public:
		EditorLayer();
		void onUpdate() override;

	private:
		std::shared_ptr<Galactose::Camera> m_camera;
	};
}
