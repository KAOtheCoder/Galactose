#pragma once

#include "Renderer/VertexArray.h"

namespace Galactose {
	enum class DataType;

	class OpenGLVertexArray : public VertexArray {
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray() override;

		void bind() override;
		void unbind() override;

		void addVertexBuffer(const std::shared_ptr<VertexBuffer>& a_buffer) override;
		void setIndexBuffer(const std::shared_ptr<IndexBuffer>& a_buffer) override;

	private:
		static unsigned int toGLType(const DataType type);

		uint32_t m_rendererId;
		uint32_t m_attributeIndex = 0;
	};
}