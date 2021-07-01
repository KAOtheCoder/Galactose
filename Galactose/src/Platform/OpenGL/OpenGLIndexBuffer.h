#pragma once

#include "Galactose/Renderer/IndexBuffer.h"

#include <glad/glad.h>

namespace Galactose {
	class OpenGLIndexBuffer : public IndexBuffer {
	public:
		OpenGLIndexBuffer(const uint32_t* a_indices, const uint32_t count);
		~OpenGLIndexBuffer() override { glDeleteBuffers(1, &m_rendererId); }

		void bind() override { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId); }
		void unbind() override { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

	private:
		uint32_t m_rendererId;
	};
}