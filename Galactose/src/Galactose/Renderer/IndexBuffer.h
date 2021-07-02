#pragma once

#include "GalactosePCH.h"

namespace Galactose {
	class IndexBuffer {
	public:
		static std::shared_ptr<IndexBuffer> create(uint32_t* a_indices, const uint32_t a_count);
		
		IndexBuffer(const uint32_t a_count) : m_count(a_count) {}
		virtual ~IndexBuffer() = default;

		virtual void bind() = 0;
		virtual void unbind() = 0;

		uint32_t count() const { return m_count; }

	protected:
		uint32_t m_count = 0;
	};
}