#include "OpenGLVertexArray.h"
#include "Core/DataType.h"
#include "Core/Global.h"

#include <glad/glad.h>

namespace Galactose {
	unsigned int OpenGLVertexArray::toGLType(const DataType type) {
		switch (type) {
		case DataType::Float: return GL_FLOAT;
		default: GT_ASSERT(false, "Cannot convert " GT_STRINGIFY(DataType) "(" + std::to_string(int(type)) + ") to GLenum type");
		}

		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray() {
		glCreateVertexArrays(1, &m_rendererId);
	}

	OpenGLVertexArray::~OpenGLVertexArray() { glDeleteVertexArrays(1, &m_rendererId); }

	void OpenGLVertexArray::bind() { glBindVertexArray(m_rendererId); }
	void OpenGLVertexArray::unbind() { glBindVertexArray(0); }

	void OpenGLVertexArray::addVertexBuffer(const std::shared_ptr<VertexBuffer>& a_buffer) {
		glBindVertexArray(m_rendererId);
		a_buffer->bind();

		GT_ASSERT(!a_buffer->layout().empty(), "Layout is empty");

		const auto stride = a_buffer->layout().stride();

		for (const auto& element : a_buffer->layout()) {
			glEnableVertexArrayAttrib(m_rendererId, m_attributeIndex);
			const auto& typeInfo = element.typeInfo();
			glVertexAttribPointer(m_attributeIndex, typeInfo.componentCount, toGLType(typeInfo.componentType), GL_FALSE, stride, (void*)element.offset());
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
