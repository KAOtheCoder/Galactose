#include "GalactosePCH.h"
#include "OpenGL/OpenGLFrameBuffer.h"

namespace Galactose {
	std::shared_ptr<Framebuffer> Framebuffer::create(
		const int32_t a_width, 
		const int32_t a_height, 
		const std::vector<Texture::Format>& a_formats)
	{
		return std::make_shared<OpenGLFramebuffer>(a_width, a_height, a_formats);
	}

	Framebuffer::Framebuffer(const int32_t a_width, const int32_t a_height) :
		m_width(a_width),
		m_height(a_height)
	{}
}