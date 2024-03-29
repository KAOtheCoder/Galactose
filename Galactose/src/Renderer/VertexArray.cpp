#include "Renderer/OpenGL/OpenGLVertexArray.h"

namespace Galactose {
	std::shared_ptr<VertexArray> VertexArray::create() {
		return std::make_shared<OpenGLVertexArray>();
	}
}