#pragma once

#include "Renderer/Texture.h"

namespace Galactose {
	class OpenGLTexture : public Texture {
	public:
		OpenGLTexture(const std::string& filePath);
		OpenGLTexture(const int32_t width, const int32_t height, const bool useAlpha = true);

		~OpenGLTexture() override;

		void setData(const void* data) override;

		void bind() override;

	private:
		void createTexture();

		uint32_t m_rendererId = 0;
		unsigned int m_format;
	};
}
