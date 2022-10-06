#include "GalactosePCH.h"

#include "Renderer/OpenGL/OpenGLIndexBuffer.h"

namespace Galactose {
	std::shared_ptr<IndexBuffer> IndexBuffer::create(const uint32_t* a_indices, const uint32_t a_count) {
		return std::make_shared<OpenGLIndexBuffer>(a_indices, a_count);
	}
}