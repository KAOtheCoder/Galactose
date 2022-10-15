#include "GalactosePCH.h"

#include "Renderer/OpenGL/OpenGLShader.h"

namespace Galactose {
	std::shared_ptr<Shader> Shader::create(const std::string& a_name, const std::string& a_vertexSrc, const std::string& a_fragmentSrc) {
		return std::make_shared<OpenGLShader>(a_name, a_vertexSrc, a_fragmentSrc);
	}
}
