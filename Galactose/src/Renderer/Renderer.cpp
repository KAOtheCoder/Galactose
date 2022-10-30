#include "GalactosePCH.h"

#include "Renderer/OpenGL/OpenGLRenderer.h"
#include "Renderer/Shader.h"
#include "Renderer/Camera.h"

namespace Galactose {
	std::shared_ptr<Renderer> Renderer::create(const std::shared_ptr<Window>& a_window) {
		const auto& renderer = std::make_shared<OpenGLRenderer>(a_window);
		
		renderer->m_id = int32_t(s_renderers.size());
		s_renderers.push_back(renderer);

		return renderer;
	}

	Renderer::Renderer(const std::shared_ptr<Window>& a_window) :
		m_window(a_window)
	{}

	void Renderer::setViewProjection(const Camera& a_camera) {
		const auto& viewProjection = a_camera.projectionMatrix() * a_camera.viewMatrix();
		m_textureShader->bind();
		m_textureShader->setMatrix4x4("u_viewProjection", viewProjection);
		m_colorShader->bind();
		m_colorShader->setMatrix4x4("u_viewProjection", viewProjection);
	}
}