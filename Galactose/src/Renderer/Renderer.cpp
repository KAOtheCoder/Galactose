#include "OpenGL/OpenGLRenderer.h"
#include "Shader.h"
#include "Texture.h"

#include <array>

namespace Galactose {
	std::shared_ptr<Renderer> Renderer::create(const std::shared_ptr<Window>& a_window) {
		const auto& renderer = std::make_shared<OpenGLRenderer>(a_window);
		
		renderer->m_id = int32_t(s_renderers.size());
		s_renderers.push_back(renderer);

		renderer->m_defaultTexture = Texture::create(1, 1);
		const std::array<unsigned char, 4> DEFAULT_TEXTURE_DATA = { 255, 255, 255, 255 };
		renderer->m_defaultTexture->setData(DEFAULT_TEXTURE_DATA.data());

		return renderer;
	}

	Renderer::Renderer(const std::shared_ptr<Window>& a_window) :
		m_window(a_window)
	{}

	void Renderer::setViewProjection(const Matrix4x4& viewProjection) {
		m_textureShader->bind();
		m_textureShader->setMatrix4x4("u_viewProjection", viewProjection);
	}
}
