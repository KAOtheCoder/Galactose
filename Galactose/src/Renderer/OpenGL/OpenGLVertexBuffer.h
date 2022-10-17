#pragma once

#include "Renderer/VertexBuffer.h"

namespace Galactose {
	class OpenGLVertexBuffer : public VertexBuffer {
	public:
		OpenGLVertexBuffer(const void* data, const uint32_t count, const Layout& layout);

		~OpenGLVertexBuffer() override; 

		void bind() override;
		void unbind() override;

		void setData(const void* data, const uint32_t count, const uint32_t offset = 0) override;

	private:
		uint32_t m_rendererId;
	};
}