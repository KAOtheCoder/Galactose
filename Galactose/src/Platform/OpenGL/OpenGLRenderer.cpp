#include "GalactosePCH.h"

#include "OpenGLRenderer.h"

#include <GLFW/glfw3.h>

namespace Galactose {
	OpenGLRenderer::OpenGLRenderer(const std::shared_ptr<Window>& a_window) {
		auto window = static_cast<GLFWwindow*>(a_window->nativeWindow());
		GT_ASSERT(window, "Renderer needs a valid window.");

		glfwMakeContextCurrent(window);
	}
}