#include "GalactosePCH.h"
#include "OpenGLIndexBuffer.h"

#include <glad/glad.h>

namespace Galactose {
	OpenGLIndexBuffer::OpenGLIndexBuffer(const uint32_t* a_indices, const uint32_t a_count) :
		IndexBuffer(a_count)
	{
		glCreateBuffers(1, &m_rendererId);
		glBindBuffer(GL_ARRAY_BUFFER, m_rendererId);
		glBufferData(GL_ARRAY_BUFFER, a_count * sizeof(uint32_t), a_indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer() { glDeleteBuffers(1, &m_rendererId); }

	void OpenGLIndexBuffer::bind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId); }
	void OpenGLIndexBuffer::unbind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }
}