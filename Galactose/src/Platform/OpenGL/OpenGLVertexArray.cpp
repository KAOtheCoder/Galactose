#include "GalactosePCH.h"

#include "OpenGLVertexArray.h"

namespace Galactose {
	OpenGLVertexArray::OpenGLVertexArray() {
		glCreateVertexArrays(1, &m_rendererId);
	}

	void OpenGLVertexArray::addVertexBuffer(const std::shared_ptr<VertexBuffer>& a_buffer) {
		glBindVertexArray(m_rendererId);
		glEnableVertexArrayAttrib(m_rendererId, 0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

		m_vertexBuffers.emplace_back(a_buffer);
	}

	void OpenGLVertexArray::setIndexBuffer(const std::shared_ptr<IndexBuffer>& a_buffer) {
		glBindVertexArray(m_rendererId);
		a_buffer->bind();
		m_indexBuffer = a_buffer;
	}
}