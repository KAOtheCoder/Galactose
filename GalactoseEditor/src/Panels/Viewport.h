#pragma once

#include "Panel.h"

#include <memory>

namespace Galactose {
	class Framebuffer;
	class Camera;
}

namespace GalactoseEditor {
	class EditorSceneData;

	class Viewport : public Panel {
	public:
		Viewport(const std::string& title, const std::shared_ptr<EditorSceneData>& sceneData);

		virtual Galactose::Camera* getCamera() const = 0;
		void onUpdate() override;

	protected:
		std::shared_ptr<EditorSceneData> m_sceneData;
		std::shared_ptr<Galactose::Framebuffer> m_framebuffer;
	};
}
