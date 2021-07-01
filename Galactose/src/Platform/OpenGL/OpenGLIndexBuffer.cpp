#include "GalactosePCH.h"

#include "OpenGLIndexBuffer.h"

namespace Galactose {
	OpenGLIndexBuffer::OpenGLIndexBuffer(const uint32_t* a_indices, const uint32_t a_count) {
		glCreateBuffers(1, &m_rendererId);
		glBindBuffer(GL_ARRAY_BUFFER, m_rendererId);
		glBufferData(GL_ARRAY_BUFFER, a_count * sizeof(uint32_t), a_indices, GL_STATIC_DRAW);
	}
}