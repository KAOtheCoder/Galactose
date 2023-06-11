#include "Galactose/Renderer/OpenGL/OpenGLShader.h"

#include <iostream>

namespace Galactose {
	std::shared_ptr<Shader> Shader::create(const std::string& a_name, const std::string& a_vertexSrc, const std::string& a_fragmentSrc) {
		return std::make_shared<OpenGLShader>(a_name, a_vertexSrc, a_fragmentSrc);
	}

	int32_t Shader::getUniform(const std::string& a_name) const {
		const auto& iter = m_uniforms.find(a_name);
		if (iter == m_uniforms.end()) {
			std::cerr << "Could not find uniform '" << a_name << "' in shader '" << m_name << "'." << std::endl;
			return -1;
		}

		return iter->second;
	}
}
