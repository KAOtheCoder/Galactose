#pragma once

#include "Panel.h"

#include <memory>

namespace Galactose {
	class Framebuffer;
	class Camera;
}

namespace GalactoseEditor {
	class EditorContext;

	class Viewport : public Panel {
	public:
		Viewport(const std::string& title, const std::shared_ptr<EditorContext>& sceneContext);

		virtual Galactose::Camera* getCamera() const = 0;
		void onUpdate() override;

	protected:
		std::shared_ptr<EditorContext> m_editorContext;
		std::shared_ptr<Galactose::Framebuffer> m_framebuffer;
	};
}
