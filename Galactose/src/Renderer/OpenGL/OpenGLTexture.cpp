#include "GalactosePCH.h"
#include "OpenGLTexture.h"

#include <glad/glad.h>
#include <stb_image.h>

namespace Galactose {
	OpenGLTexture::OpenGLTexture(const int32_t a_width, const int32_t a_height, const bool a_useAlpha) :
		Texture(a_width, a_height, a_useAlpha),
		m_format(a_useAlpha ? GL_RGBA : GL_RGB)
	{}

	OpenGLTexture::OpenGLTexture(const std::string& a_filePath) : 
		Texture(),
		m_format(GL_RGBA)
	{
		stbi_set_flip_vertically_on_load(1);
		int channels;
		const auto data = stbi_load(a_filePath.c_str(), &m_width, &m_height, &channels, 0);

		if (data) {
			switch (channels) {
			case 3: m_format = GL_RGB;
				break;
			case 4: m_format = GL_RGBA;
				break;
			default: std::cerr << "Unsupported texture format of '" << a_filePath << "'." << std::endl;
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
		glBindTexture(GL_TEXTURE_2D, m_rendererId);
		glTextureStorage2D(m_rendererId, 1, m_format == GL_RGB ? GL_RGB8 : GL_RGBA8, m_width, m_height);
		glTexParameteri(m_rendererId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(m_rendererId, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(m_rendererId, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_rendererId, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	void OpenGLTexture::setData(const void* a_data) {
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, m_format, GL_UNSIGNED_BYTE, a_data);
		m_loaded = true;
	}

	void OpenGLTexture::bind() { glBindTextureUnit(0, m_rendererId); }
}
