#pragma once

#include "Galactose/Renderer/Renderer.h"

namespace Galactose {
	class OpenGLRenderer : public Renderer {
	public:
		OpenGLRenderer(const std::shared_ptr<Window>& a_window);
	};
}