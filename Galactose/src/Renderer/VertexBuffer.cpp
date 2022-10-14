#include "GalactosePCH.h"

#include "VertexBuffer.h"
#include "Renderer/OpenGL/OpenGLVertexBuffer.h"

namespace Galactose {
	std::shared_ptr<VertexBuffer> VertexBuffer::create(const void* a_data, const uint32_t a_byteSize) {
		return std::make_shared<OpenGLVertexBuffer>(a_data, a_byteSize);
	}

	std::shared_ptr<VertexBuffer> VertexBuffer::create(const void* a_data, const uint32_t a_count, const Layout& a_layout) {
		return std::make_shared<OpenGLVertexBuffer>(a_data, a_count, a_layout);
	}

	VertexBuffer::Layout::Attribute::Attribute(const std::string& a_name, const ShaderTypeInfo::Type a_type) :
		m_name(a_name),
		m_type(a_type),
		m_offset(0)
	{}

	VertexBuffer::Layout::Layout(std::initializer_list<Attribute> elements) :
		m_elements(elements),
		m_stride(0)
	{
		int32_t offset = 0;

		for (auto& element : m_elements) {
			element.m_offset = offset;
			offset += element.typeInfo().byteSize;
		}
		
		m_stride = offset;
	}

	void VertexBuffer::setLayout(const Layout& a_layout) {
		GT_ASSERT(!a_layout.empty(), "Layout is empty");
		m_layout = a_layout;
	}
}
