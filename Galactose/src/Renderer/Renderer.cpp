#include "GalactosePCH.h"

#include "Renderer/OpenGL/OpenGLRenderer.h"

namespace Galactose {
	std::shared_ptr<Renderer> Renderer::create(const std::shared_ptr<Window>& a_window) {
		const auto& renderer = std::make_shared<OpenGLRenderer>(a_window);
		
		renderer->m_id = int32_t(s_renderers.size());
		s_renderers.push_back(renderer);

		return renderer;
	}
}