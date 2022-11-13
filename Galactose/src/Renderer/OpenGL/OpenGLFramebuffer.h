#pragma once

#include "Renderer/FrameBuffer.h"

namespace Galactose {
	class OpenGLFramebuffer : public Framebuffer {
	public:
		OpenGLFramebuffer(const int32_t width, const int32_t height, const std::vector<Texture::Format>& formats);
		~OpenGLFramebuffer() override;

		void bind() override;
		void unbind() override;
		void resize(const int32_t width, const int32_t height) override;

	private:
		void createFramebuffer(const std::vector<Texture::Format>& formats);
		void deleteFramebuffer();

		uint32_t m_rendererId = 0;
	};
}
