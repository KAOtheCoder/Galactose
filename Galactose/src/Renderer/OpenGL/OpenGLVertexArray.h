#pragma once

#include "Core/DataType.h"
#include "Renderer/VertexArray.h"

namespace Galactose {
	class OpenGLVertexArray : public VertexArray {
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray() override;

		void bind() override;
		void unbind() override;

		void addVertexBuffer(const std::shared_ptr<VertexBuffer>& a_buffer) override;
		void setIndexBuffer(const std::shared_ptr<IndexBuffer>& a_buffer) override;

	private:
		static unsigned int toGLType(const DataType::Type type);

		uint32_t m_rendererId;
		uint32_t m_attributeIndex = 0;
	};
}