#pragma once

#include "Renderer/Texture.h"

namespace Galactose {
	class OpenGLTexture : public Texture {
	public:
		OpenGLTexture(const std::string& filePath);
		OpenGLTexture(const int32_t width, const int32_t height, const Format a_format = Texture::RGBA8);

		~OpenGLTexture() override;

		uint32_t rendererId() const { return m_rendererId; }

		void setData(const void* data) override;
		void bind(const uint8_t slot) override;

	private:
		static unsigned int toGLFormat(const Texture::Format format);
		static unsigned int toGLInternalFormat(const Texture::Format format);

		void createTexture();

		uint32_t m_rendererId = 0;
	};
}
