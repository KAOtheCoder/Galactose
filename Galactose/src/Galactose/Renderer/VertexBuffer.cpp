#include "GalactosePCH.h"

#include "VertexBuffer.h"
#include "Platform/OpenGL/OpenGLVertexBuffer.h"

namespace Galactose {
	std::shared_ptr<VertexBuffer> VertexBuffer::create(const void* a_data, const uint32_t a_byteSize) {
		return std::make_shared<OpenGLVertexBuffer>(a_data, a_byteSize);
	}
}
