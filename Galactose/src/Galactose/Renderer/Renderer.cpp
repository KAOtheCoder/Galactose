#include "GalactosePCH.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLRenderer.h"

namespace Galactose {
	std::shared_ptr<Renderer> Renderer::create(const std::shared_ptr<Window>& a_window) {
		return std::make_shared<OpenGLRenderer>(a_window);
	}
}