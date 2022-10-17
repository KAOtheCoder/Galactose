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

		glClearColor(0.1f, 0.05f, 0.2f, 1.0f);

		std::string vertexSrc = 
#include "shaders/TextureVertex.glsl"
			;

		std::string fragmentSrc = 
#include "shaders/TextureFragment.glsl"
			;

		m_textureShader = std::make_shared<OpenGLShader>("texture", vertexSrc, fragmentSrc);

		vertexSrc =
#include "shaders/ColorVertex.glsl"
			;

		fragmentSrc =
#include "shaders/ColorFragment.glsl"
			;

		m_colorShader = std::make_shared<OpenGLShader>("color", vertexSrc, fragmentSrc);

		m_quadVertexArray = VertexArray::create();
		const auto& layout = VertexBuffer::Layout({ { "position", DataType::Vector3 }, { "uv", DataType::Vector2 } });
		m_quadVertexBuffer = VertexBuffer::create(nullptr, 4, layout);
		m_quadVertexArray->addVertexBuffer(m_quadVertexBuffer);

		const std::array<uint32_t, 6> SPRITE_INDICES = {
			0, 1, 2,
			2, 3, 0
		};

		m_quadVertexArray->setIndexBuffer(IndexBuffer::create(SPRITE_INDICES.data(), uint32_t(SPRITE_INDICES.size())));
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
		a_texture->bind(0);
		m_textureShader->bind();
		drawQuad(a_center, a_size);
	}

	void OpenGLRenderer::drawQuad(const Vector3& a_center, const Vector2& a_size, const Vector4& a_color) {
		m_colorShader->bind();
		m_colorShader->setVector4("u_color", a_color);
		drawQuad(a_center, a_size);
	}

	void OpenGLRenderer::drawQuad(const Vector3& a_center, const Vector2& a_size) {
		const auto& halfSize = a_size / 2.0f;

		const float vertices[] = {
			a_center.x - halfSize.x, a_center.y - halfSize.y, a_center.z, 0, 0,
			a_center.x + halfSize.x, a_center.y - halfSize.y, a_center.z, 1, 0,
			a_center.x + halfSize.x, a_center.y + halfSize.y, a_center.z, 1, 1,
			a_center.x - halfSize.x, a_center.y + halfSize.y, a_center.z, 0, 1
		};

		m_quadVertexBuffer->setData(vertices, 4);
		drawVertexArrayIndexed(m_quadVertexArray);
	}
}
