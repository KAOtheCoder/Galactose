#include "GalactosePCH.h"

#include "Renderer/OpenGL/OpenGLShader.h"

namespace Galactose {
	std::shared_ptr<Shader> Shader::create() {
		return std::make_shared<OpenGLShader>();
	}
}