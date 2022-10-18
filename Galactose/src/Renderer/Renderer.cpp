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

	void Renderer::setViewProjection(const Camera& a_camera) {
		const auto& vp = a_camera.projectionMatrix() * a_camera.viewMatrix();
		m_textureShader->setMatrix4x4("u_viewProjection", vp);
		m_colorShader->setMatrix4x4("u_viewProjection", vp);
	}
}