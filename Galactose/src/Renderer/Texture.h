#pragma once

#include "GalactosePCH.h"

namespace Galactose {
	class Texture {
	public:
		static std::shared_ptr<Texture> create(const std::string& filePath);
		static std::shared_ptr<Texture> create(const int32_t width, const int32_t height, const bool useAlpha = true);

		virtual ~Texture() = default;

		bool hasAlphaChannel() const { return m_alphaChannel; }
		bool isLoaded() const { return m_loaded; }

		virtual void setData(const void* data) = 0;

		virtual void bind() = 0;
		virtual void unbind() = 0;

	protected:
		Texture() = default;
		Texture(const int32_t width, const int32_t height, const bool useAlpha = true);

		int32_t m_width = 0;
		int32_t m_height = 0;
		bool m_alphaChannel = true;
		bool m_loaded = false;
	};
}
