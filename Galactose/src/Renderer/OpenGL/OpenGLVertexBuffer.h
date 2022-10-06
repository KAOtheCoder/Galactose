#pragma once

#include "Renderer/VertexBuffer.h"

#include <glad/glad.h>

namespace Galactose {
	class OpenGLVertexBuffer : public VertexBuffer {
	public:
		OpenGLVertexBuffer(const void* a_data, const uint32_t a_byteSize);
		~OpenGLVertexBuffer() override { glDeleteBuffers(1, &m_rendererId); }

		void bind() override { glBindBuffer(GL_ARRAY_BUFFER, m_rendererId); }
		void unbind() override { glBindBuffer(GL_ARRAY_BUFFER, 0); }

	private:
		uint32_t m_rendererId;
	};
}