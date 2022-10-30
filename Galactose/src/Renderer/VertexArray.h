#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace Galactose {
	class VertexArray {
	public:
		static std::shared_ptr<VertexArray> create();
		virtual ~VertexArray() = default;

		virtual void bind() = 0;
		virtual void unbind() = 0;

		virtual void addVertexBuffer(const std::shared_ptr<VertexBuffer>& a_buffer) = 0;
		virtual void setIndexBuffer(const std::shared_ptr<IndexBuffer>& a_buffer) = 0;

		std::shared_ptr<VertexBuffer> vertexBuffer(const int a_index) const { return m_vertexBuffers[a_index]; }
		std::shared_ptr<IndexBuffer> indexBuffer() const { return m_indexBuffer; }

	protected:
		std::vector<std::shared_ptr<VertexBuffer>> m_vertexBuffers;
		std::shared_ptr<IndexBuffer> m_indexBuffer;
	};
}