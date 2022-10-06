#include "GalactosePCH.h"

#include "Renderer/OpenGL/OpenGLVertexBuffer.h"

#include <glad/glad.h>

namespace Galactose {
	OpenGLVertexBuffer::OpenGLVertexBuffer(const void* a_data, const uint32_t a_byteSize) {
		glCreateBuffers(1, &m_rendererId);
		glBindBuffer(GL_ARRAY_BUFFER, m_rendererId);
		glBufferData(GL_ARRAY_BUFFER, a_byteSize, a_data, GL_STATIC_DRAW);
	}
}