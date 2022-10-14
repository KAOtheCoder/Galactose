#include "GalactosePCH.h"

#include "OpenGLVertexArray.h"

namespace Galactose {
	OpenGLVertexArray::OpenGLVertexArray() {
		glCreateVertexArrays(1, &m_rendererId);
	}

	void OpenGLVertexArray::addVertexBuffer(const std::shared_ptr<VertexBuffer>& a_buffer) {
		glBindVertexArray(m_rendererId);
		a_buffer->bind();
		//glEnableVertexArrayAttrib(m_rendererId, 0);
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

		const auto stride = a_buffer->layout().stride();

		GT_ASSERT(!a_buffer->layout().empty(), "Layout is empty");

		for (const auto& element : a_buffer->layout()) {
			glEnableVertexArrayAttrib(m_rendererId, m_attributeIndex);
			glVertexAttribPointer(m_attributeIndex, element.typeInfo().componentCount, GL_FLOAT, GL_FALSE, stride, (void*)element.offset());
			++m_attributeIndex;
		}

		m_vertexBuffers.push_back(a_buffer);
	}

	void OpenGLVertexArray::setIndexBuffer(const std::shared_ptr<IndexBuffer>& a_buffer) {
		glBindVertexArray(m_rendererId);
		a_buffer->bind();
		m_indexBuffer = a_buffer;
	}
}
