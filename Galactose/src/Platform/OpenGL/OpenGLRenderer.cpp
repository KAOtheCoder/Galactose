#include "GalactosePCH.h"

#include "OpenGLRenderer.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Galactose {
	OpenGLRenderer::OpenGLRenderer(const std::shared_ptr<Window>& a_window) {
		auto window = static_cast<GLFWwindow*>(a_window->nativeWindow());
		GT_ASSERT(window, "Renderer needs a valid window.");

		glfwMakeContextCurrent(window);
		const int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		GT_ASSERT(status, "Failed to initialize Glad.");

		std::cout << "Renderer API: OpenGL" << std::endl
			<< "Version: " << glGetString(GL_VERSION) << std::endl
			<< "Vendor: " << glGetString(GL_VENDOR) << std::endl
			<< "Renderer: " << glGetString(GL_RENDERER) << std::endl;

		glClearColor(0.1f, 0.05f, 0.2f, 1.0f);
	}

	void OpenGLRenderer::drawVertexArrayIndexed(const std::shared_ptr<VertexArray>& a_vertexArray) {
		a_vertexArray.get()->bind();
		glDrawElements(GL_TRIANGLES, a_vertexArray.get()->indexBuffer()->count(), GL_UNSIGNED_INT, nullptr);
	}
}