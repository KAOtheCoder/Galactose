#include "OpenGLTexture.h"
#include "Core/Global.h"

#include <glad/glad.h>
#include <stb_image.h>

namespace Galactose {
	unsigned int OpenGLTexture::toGLFormat(const Texture::Format a_format) {
		switch (a_format) {
		case Texture::RGB8: return GL_RGB;
		case Texture::RGBA8: return GL_RGBA;
		case Texture::Depth24: return GL_DEPTH_COMPONENT;
		case Texture::Depth24Stencil8: return GL_DEPTH_STENCIL;
		default:
			GT_ASSERT(false, "Unknown texture format: " + std::to_string(a_format));
		}

		return 0;
	}

	unsigned int OpenGLTexture::toGLInternalFormat(const Texture::Format a_format) {
		switch (a_format) {
		case Texture::RGB8: return GL_RGB8;
		case Texture::RGBA8: return GL_RGBA8;
		case Texture::Depth24: return GL_DEPTH_COMPONENT24;
		case Texture::Depth24Stencil8: return GL_DEPTH24_STENCIL8;
		default:
			GT_ASSERT(false, "Unknown texture format: " + std::to_string(a_format));
		}

		return 0;
	}

	OpenGLTexture::OpenGLTexture(const int32_t a_width, const int32_t a_height, const Format a_format) :
		Texture(a_width, a_height, a_format)
	{
		createTexture();
	}

	OpenGLTexture::OpenGLTexture(const std::string& a_filePath) : 
		Texture(a_filePath)
	{
		stbi_set_flip_vertically_on_load(1);
		int channels;
		const auto data = stbi_load(a_filePath.c_str(), &m_width, &m_height, &channels, 0);

		if (data) {
			switch (channels) {
			case 3: m_format = RGB8;
				break;
			case 4: m_format = RGBA8;
				break;
			default: std::cerr << "Failed to load '" << a_filePath << "': " << channels << " channel textures not supported." << std::endl;
				stbi_image_free(data);
				return;
			}

			createTexture();
			setData(data);
			stbi_image_free(data);
		}
		else {
			std::cerr << "Failed to load texture source: " << a_filePath << std::endl;
			std::cerr << stbi_failure_reason() << std::endl;
		}
	}

	OpenGLTexture::~OpenGLTexture() { glDeleteTextures(1, &m_rendererId); }

	void OpenGLTexture::createTexture() {
		glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererId);
		glBindTexture(GL_TEXTURE_2D, m_rendererId); // Dont remove this line, or texture won't show up
		glTextureStorage2D(m_rendererId, 1, toGLInternalFormat(m_format), m_width, m_height);
		glTexParameteri(m_rendererId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(m_rendererId, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}

	void OpenGLTexture::setData(const void* a_data) {
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, toGLFormat(m_format), GL_UNSIGNED_BYTE, a_data);
		m_loaded = true;
	}

	void OpenGLTexture::bind(const uint8_t a_slot) { glBindTextureUnit(a_slot, m_rendererId); }
}
