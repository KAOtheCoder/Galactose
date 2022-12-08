#include "OpenGLRenderer.h"
#include "OpenGLShader.h"
#include "Core/DataType.h"
#include "Renderer/Texture.h"
#include "Core/Global.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Galactose {
	OpenGLRenderer::OpenGLRenderer(const std::shared_ptr<Window>& a_window) :
		Renderer(a_window)
	{
		auto window = static_cast<GLFWwindow*>(m_window->nativeWindow());
		GT_ASSERT(window, "Renderer needs a valid window.");

		glfwMakeContextCurrent(window);
		const int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		GT_ASSERT(status, "Failed to initialize Glad.");

		std::cout << "Renderer API: OpenGL" << std::endl
			<< "Version: " << glGetString(GL_VERSION) << std::endl
			<< "Vendor: " << glGetString(GL_VENDOR) << std::endl
			<< "Renderer: " << glGetString(GL_RENDERER) << std::endl;

//#ifdef GT_DEBUG
//		glEnable(GL_DEBUG_OUTPUT);
//		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
//#endif

		glEnable(GL_DEPTH_TEST);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &m_textureSlots);

		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		std::string vertexSrc = 
#include "shaders/QuadTextureVertex.glsl"
			;

		std::string fragmentSrc = 
#include "shaders/QuadTextureFragment.glsl"
			;

		m_textureShader = std::make_shared<OpenGLShader>("QuadTexture", vertexSrc, fragmentSrc);

		vertexSrc =
#include "shaders/QuadColorVertex.glsl"
			;

		fragmentSrc =
#include "shaders/QuadColorFragment.glsl"
			;

		m_colorShader = std::make_shared<OpenGLShader>("QuadColor", vertexSrc, fragmentSrc);

		vertexSrc =
#include "shaders/Quad2DVertex.glsl"
			;

		fragmentSrc =
#include "shaders/QuadTextureFragment.glsl"
			;

		m_quad2DShader = std::make_shared<OpenGLShader>("Quad2D", vertexSrc, fragmentSrc);

		m_quadVertexArray = VertexArray::create();
		const auto& quadLayout = VertexBuffer::Layout({ { "position", DataType::Vector3 }, { "uv", DataType::Vector2 } });
		m_quadVertexArray->addVertexBuffer(VertexBuffer::create(nullptr, 4, quadLayout));

		const std::array<uint32_t, 6> SPRITE_INDICES = {
			0, 1, 2,
			2, 3, 0
		};

		m_quadVertexArray->setIndexBuffer(IndexBuffer::create(SPRITE_INDICES.data(), uint32_t(SPRITE_INDICES.size())));
		
		m_quad2DVertexArray = VertexArray::create();
		const auto& quadLayout2D = VertexBuffer::Layout({ { "position", DataType::Vector2 }, { "uv", DataType::Vector2 } });
		m_quad2DVertexArray->addVertexBuffer(VertexBuffer::create(nullptr, 4, quadLayout2D));
		m_quad2DVertexArray->setIndexBuffer(IndexBuffer::create(SPRITE_INDICES.data(), uint32_t(SPRITE_INDICES.size())));
	}

	void OpenGLRenderer::setClearColor(const Vector4& color) {
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRenderer::clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRenderer::drawVertexArrayIndexed(const std::shared_ptr<VertexArray>& a_vertexArray) {
		a_vertexArray->bind();
		glDrawElements(GL_TRIANGLES, a_vertexArray->indexBuffer()->count(), GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRenderer::drawSprite(const Matrix4x4& a_transform, const Sprite& a_sprite) {
		if (a_sprite.texture()) {
			//TO DO: tint texture
			m_textureShader->bind();
			a_sprite.texture()->bind(0);
			m_textureShader->setInt("u_texture", 0);
		}
		else {
			m_colorShader->bind();
			m_colorShader->setVector4("u_color", a_sprite.color());
		}

		drawSprite(a_transform, a_sprite.size(), a_sprite.pivot());
	}

	void OpenGLRenderer::drawSprite(const Matrix4x4& a_transform, const Vector2& a_size, const Vector2& a_pivot) {
		const auto& reversePivot = Vector2(1, 1) - a_pivot;
		const Vector2 minCorner = Vector2(-a_pivot.x * a_size.x, -a_pivot.y * a_size.y);
		const Vector2 maxCorner = Vector2(reversePivot.x * a_size.x, reversePivot.y * a_size.y);

		const Vector3 topLeft = a_transform * Vector4(minCorner, 0, 1);
		const Vector3 topRight = a_transform * Vector4(maxCorner.x, minCorner.y, 0, 1);
		const Vector3 bottomRight = a_transform * Vector4(maxCorner, 0, 1);
		const Vector3 bottomLeft = a_transform * Vector4(minCorner.x, maxCorner.y, 0, 1);

		const float vertices[] = {
			topLeft.x, topLeft.y, topLeft.z, 0, 0,
			topRight.x, topRight.y, topRight.z, 1, 0,
			bottomRight.x, bottomRight.y, bottomRight.z, 1, 1,
			bottomLeft.x, bottomLeft.y, bottomLeft.z, 0, 1
		};

		m_quadVertexArray->vertexBuffer(0)->setData(vertices, 4);
		drawVertexArrayIndexed(m_quadVertexArray);
	}

	void OpenGLRenderer::drawQuad2D(const Vector2& a_topLeft, const Vector2& a_size, const std::shared_ptr<Texture>& a_texture, const Vector2& a_canvasSize) {
		glDisable(GL_DEPTH_TEST);
		m_quad2DShader->bind();
		a_texture->bind(0);
		m_quad2DShader->setInt("u_texture", 0);
		m_quad2DShader->setVector2("u_canvasSize", a_canvasSize);

		const float vertices[] = { // texture y axis reverted
			a_topLeft.x, a_topLeft.y, 0, 1,
			a_topLeft.x + a_size.x, a_topLeft.y, 1, 1,
			a_topLeft.x + a_size.x, a_topLeft.y + a_size.y, 1, 0,
			a_topLeft.x, a_topLeft.y + a_size.y, 0, 0
		};

		m_quad2DVertexArray->vertexBuffer(0)->setData(vertices, 4);
		drawVertexArrayIndexed(m_quad2DVertexArray);
		glEnable(GL_DEPTH_TEST);
	}
}
