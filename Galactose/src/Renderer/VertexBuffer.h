#pragma once

#include "GalactosePCH.h"
#include "Core//DataType.h"

namespace Galactose {
	class VertexBuffer {
	public:
		static std::shared_ptr<VertexBuffer> create(const void* a_data, const uint32_t a_byteSize);
		virtual ~VertexBuffer() = default;

		virtual void bind() = 0;
		virtual void unbind() = 0;

		/*class Layout {
		private:
			std::vector<DataType> m_layout;
		};
	private:
		Layout m_layout;*/
	};
}