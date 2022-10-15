#include "GalactosePCH.h"

#include "OpenGLRenderer.h"
#include "OpenGLShader.h"
#include "Core/DataType.h"

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

		m_shader = std::make_shared<OpenGLShader>();
	}

	void OpenGLRenderer::drawVertexArrayIndexed(const std::shared_ptr<VertexArray>& a_vertexArray) {
		a_vertexArray->bind();
		glDrawElements(GL_TRIANGLES, a_vertexArray->indexBuffer()->count(), GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRenderer::drawSprite(const Vector3 a_center, const Vector2 a_size) {
		auto vertexArray = VertexArray::create();
		const auto& halfSize = a_size / 2.0f;

		float vertices[] = {
			a_center.x - halfSize.x, a_center.y - halfSize.y, a_center.z,
			a_center.x + halfSize.x, a_center.y - halfSize.y, a_center.z,
			a_center.x + halfSize.x, a_center.y + halfSize.y, a_center.z,
			a_center.x - halfSize.x, a_center.y + halfSize.y, a_center.z
		};

		uint32_t indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		auto vertex_buffer = VertexBuffer::create(vertices, 4, VertexBuffer::Layout({ { "position", DataType::Vector3 }}));
		vertexArray->addVertexBuffer(vertex_buffer);
		vertexArray->setIndexBuffer(IndexBuffer::create(indices, 6));
		m_shader->bind();
		drawVertexArrayIndexed(vertexArray);
	}
}
