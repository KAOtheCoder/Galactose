#include "GalactosePCH.h"

#include "OpenGLRenderer.h"
#include "OpenGLShader.h"
#include "Core/DataType.h"
#include "Renderer/Texture.h"

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

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &m_textureSlots);

		for (int i = 0; i < m_textureSlots; ++i)
			glActiveTexture(GL_TEXTURE0 + i);

		glClearColor(0.1f, 0.05f, 0.2f, 1.0f);

		const std::string& vertexSrc = R"(
			#version 410 core

			layout(location = 0) in vec3 i_position;
			layout(location = 1) in vec2 i_uv;

			layout(location = 0) out vec2 o_uv;

			uniform mat4 u_mvp;

			void main() {
				gl_Position = u_mvp * vec4(i_position, 1.0);
				o_uv = i_uv;
			}
		)";

		const std::string& fragmentSrc = R"(
			#version 410 core

			layout(location = 0) in vec2 i_uv;

			layout(location = 0) out vec4 o_color;

			uniform sampler2D u_texture;

			void main() {
				o_color = texture(u_texture, i_uv).rgba;
			}
		)";

		m_shader = std::make_shared<OpenGLShader>("sprite", vertexSrc, fragmentSrc);
	}

	void OpenGLRenderer::setClearColor(const float a_r, const float a_g, const float a_b, const float a_a) {
		glClearColor(a_r, a_g, a_b, a_a);
	}

	void OpenGLRenderer::clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRenderer::drawVertexArrayIndexed(const std::shared_ptr<VertexArray>& a_vertexArray) {
		a_vertexArray->bind();
		glDrawElements(GL_TRIANGLES, a_vertexArray->indexBuffer()->count(), GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRenderer::drawSprite(const Vector3& a_center, const Vector2& a_size, const std::shared_ptr<Texture>& a_texture) {
		auto vertexArray = VertexArray::create();
		const auto& halfSize = a_size / 2.0f;

		float vertices[] = {
			a_center.x - halfSize.x, a_center.y - halfSize.y, a_center.z, 0, 0,
			a_center.x + halfSize.x, a_center.y - halfSize.y, a_center.z, 1, 0,
			a_center.x + halfSize.x, a_center.y + halfSize.y, a_center.z, 1, 1,
			a_center.x - halfSize.x, a_center.y + halfSize.y, a_center.z, 0, 1
		};

		uint32_t indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		const auto& layout = VertexBuffer::Layout({ { "position", DataType::Vector3 }, { "uv", DataType::Vector2 } });
		auto vertex_buffer = VertexBuffer::create(vertices, 4, VertexBuffer::Layout(layout));
		vertexArray->addVertexBuffer(vertex_buffer);
		vertexArray->setIndexBuffer(IndexBuffer::create(indices, 6));
		m_shader->bind();
		a_texture->bind(0);
		drawVertexArrayIndexed(vertexArray);
	}
}
