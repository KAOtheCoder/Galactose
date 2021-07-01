#include "GalactosePCH.h"

#include "Platform/OpenGL/OpenGLRenderer.h"

namespace Galactose {
	std::shared_ptr<Renderer> Renderer::create(const std::shared_ptr<Window>& a_window) {
		auto& renderer = std::make_shared<OpenGLRenderer>(a_window);
		
		renderer.get()->m_id = int32_t(s_renderers.size());
		s_renderers.emplace_back(renderer);

		return renderer;
	}
}