#pragma once

#include "Galactose/Core/Global.h"

#include <memory>
#include <filesystem>

namespace Galactose {
	class Texture {
	public:
		enum Format {
			RGB8,
			RGBA8,
			Depth24,
			Depth24Stencil8
		};

		GT_API static std::shared_ptr<Texture> create(const std::filesystem::path& filePath);
		static std::shared_ptr<Texture> create(const int32_t width, const int32_t height, const Format format = RGBA8);

		virtual ~Texture() = default;

		uint32_t rendererId() const { return m_rendererId; }

		int32_t width() const { return m_width; }
		int32_t height() const { return m_height; }

		Format format() const { return m_format; }
		bool isLoaded() const { return m_loaded; }

		std::filesystem::path filePath() const { return m_filePath; }

		virtual void setData(const void* data) = 0;

		virtual void bind(const uint8_t slot) = 0;

	protected:
		Texture(const std::filesystem::path& a_filePath) : m_filePath(a_filePath) {}
		Texture(const int32_t width, const int32_t height, const Format format = RGBA8);

		uint32_t m_rendererId = 0;
		int32_t m_width = 0;
		int32_t m_height = 0;
		Format m_format = RGBA8;
		bool m_loaded = false;
		std::filesystem::path m_filePath;
	};
}
