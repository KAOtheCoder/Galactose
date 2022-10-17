#include "GalactosePCH.h"
#include "Renderer/OpenGL/OpenGLVertexBuffer.h"

#include <glad/glad.h>

namespace Galactose {
	OpenGLVertexBuffer::OpenGLVertexBuffer(const void* a_data, const uint32_t a_count, const Layout& a_layout) :
		VertexBuffer(a_layout)
	{
		glCreateBuffers(1, &m_rendererId);
		glBindBuffer(GL_ARRAY_BUFFER, m_rendererId);
		glBufferData(GL_ARRAY_BUFFER, a_count * a_layout.stride(), a_data, GL_DYNAMIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer() { glDeleteBuffers(1, &m_rendererId); }

	void OpenGLVertexBuffer::setData(const void* a_data, const uint32_t a_count, const uint32_t a_offset) {
		const auto stride = layout().stride();
		glBindBuffer(GL_ARRAY_BUFFER, m_rendererId);
		glBufferSubData(GL_ARRAY_BUFFER, a_offset * stride, a_count * stride, a_data);
	};

	void OpenGLVertexBuffer::bind() { glBindBuffer(GL_ARRAY_BUFFER, m_rendererId); }
	void OpenGLVertexBuffer::unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }
}
