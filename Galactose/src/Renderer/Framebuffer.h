#pragma once

#include "Texture.h"

#include <vector>

namespace Galactose {
	class Framebuffer {
	public:
		GT_API static std::shared_ptr<Framebuffer> create(
			const int32_t width, 
			const int32_t height, 
			const std::vector<Texture::Format>& formats
		);

		virtual ~Framebuffer() = default;

		std::shared_ptr<Texture> texture(const int a_index) const { return m_textures[a_index]; }

		GT_API virtual void bind() = 0;
		GT_API virtual void unbind() = 0;
		GT_API virtual void resize(const int32_t width, const int32_t height) = 0;

	protected:
		Framebuffer(const int32_t width, const int32_t height);

		int32_t m_width;
		int32_t m_height;
		std::vector<std::shared_ptr<Texture>> m_textures;
	};
}
