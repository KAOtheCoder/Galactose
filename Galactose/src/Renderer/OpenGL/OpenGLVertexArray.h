#pragma once

#include "Renderer/VertexArray.h"

#include <glad/glad.h>

namespace Galactose {
	class OpenGLVertexArray : public VertexArray {
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray() override { glDeleteVertexArrays(1, &m_rendererId); }

		void bind() override { glBindVertexArray(m_rendererId); }
		void unbind() override { glBindVertexArray(0); }

		void addVertexBuffer(const std::shared_ptr<VertexBuffer>& a_buffer) override;
		void setIndexBuffer(const std::shared_ptr<IndexBuffer>& a_buffer) override;

	private:
		uint32_t m_rendererId;
		uint32_t m_attributeIndex = 0;
	};
}