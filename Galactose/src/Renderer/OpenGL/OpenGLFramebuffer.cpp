#include "GalactosePCH.h"
#include "OpenGLFrameBuffer.h"
#include "OpenGLTexture.h"

#include <glad/glad.h>

namespace Galactose {
	OpenGLFramebuffer::OpenGLFramebuffer(
		const int32_t a_width, 
		const int32_t a_height, 
		const std::vector<Texture::Format>& a_formats) :
		Framebuffer(a_width, a_height)
	{
		glCreateFramebuffers(1, &m_rendererId);
		glBindFramebuffer(GL_FRAMEBUFFER, m_rendererId);

		const auto formatsSize = a_formats.size();
		std::vector<GLenum> buffers(formatsSize, GL_NONE);

		for (size_t i = 0; i < formatsSize; ++i) {
			const auto& texture = std::make_shared<OpenGLTexture>(m_width, m_height, a_formats[i]);
			m_textures.push_back(texture);
			glBindTexture(GL_TEXTURE_2D, texture->rendererId());
			glBindFramebuffer(GL_FRAMEBUFFER, m_rendererId);

			GLenum attachment;
			switch (a_formats[i]) {
			case Texture::RGB8:
			case Texture::RGBA8: attachment = GL_COLOR_ATTACHMENT0 + i;
				buffers[i] = attachment;
				break;
			case Texture::Depth24: attachment = GL_DEPTH_ATTACHMENT;
				break;
			case Texture::Depth24Stencil8: attachment = GL_DEPTH_STENCIL_ATTACHMENT;
				break;
			default:
				GT_ASSERT(false, "Unknown texture format: " + std::to_string(a_formats[i]));
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture->rendererId(), 0);
		}

		glDrawBuffers(formatsSize, buffers.data());

		GT_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete.");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	OpenGLFramebuffer::~OpenGLFramebuffer() { 
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDeleteFramebuffers(1, &m_rendererId);
	}

	void OpenGLFramebuffer::bind() { 
		glBindFramebuffer(GL_FRAMEBUFFER, m_rendererId); 
		glViewport(0, 0, m_width, m_height);
	}

	void OpenGLFramebuffer::unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
}
