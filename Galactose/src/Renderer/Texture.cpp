#include "GalactosePCH.h"
#include "OpenGL/OpenGLTexture.h"

namespace Galactose {
	std::shared_ptr<Texture> Texture::create(const std::string& a_filePath) {
		return std::make_shared<OpenGLTexture>(a_filePath);
	}

	static std::shared_ptr<Texture> create(const int32_t a_width, const int32_t a_height, const bool a_useAlpha = true) {
		return std::make_shared<OpenGLTexture>(a_width, a_height, a_useAlpha);
	}

	Texture::Texture(const int32_t a_width, const int32_t a_height, const bool a_useAlpha) :
		m_width(a_width),
		m_height(a_height),
		m_alphaChannel(a_useAlpha)
	{}
}
