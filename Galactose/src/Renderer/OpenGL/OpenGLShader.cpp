#include "GalactosePCH.h"
#include "OpenGLShader.h"
#include "Core/Math.h"

#include <glad/glad.h>

namespace Galactose {
	OpenGLShader::OpenGLShader(const std::string& a_name, const std::string& a_vertexSrc, const std::string& a_fragmentSrc) :
		Shader(a_name) 
	{
		struct ShaderData {
			GLuint id = 0;
			GLenum type;
			std::string source;
		};

		std::array<ShaderData, 2> shaderDatas = { {
			{ 0, GL_VERTEX_SHADER, a_vertexSrc }, { 0, GL_FRAGMENT_SHADER, a_fragmentSrc }
		} };

		for (auto& shaderData : shaderDatas) {
			shaderData.id = glCreateShader(shaderData.type);

			const GLchar* source = shaderData.source.c_str();
			glShaderSource(shaderData.id, 1, &source, 0);

			glCompileShader(shaderData.id);
			GLint isCompiled = 0;
			glGetShaderiv(shaderData.id, GL_COMPILE_STATUS, &isCompiled);

			if (isCompiled == GL_FALSE) {
				GLint infoLength = 0;
				glGetShaderiv(shaderData.id, GL_INFO_LOG_LENGTH, &infoLength);

				std::vector<GLchar> infoLog(infoLength);
				glGetShaderInfoLog(shaderData.id, infoLength, &infoLength, infoLog.data());

				glDeleteShader(shaderData.id);

				std::cerr << infoLog.data();
				GT_ASSERT(false, "Shader compilation failed.");
				return;
			}
		}

		m_rendererId = glCreateProgram();

		for (const auto& shaderData : shaderDatas)
			glAttachShader(m_rendererId, shaderData.id);

		glLinkProgram(m_rendererId);

		GLint isLinked = 0;
		glGetProgramiv(m_rendererId, GL_LINK_STATUS, &isLinked);

		if (isLinked == GL_FALSE) {
			GLint infoLength = 0;
			glGetProgramiv(m_rendererId, GL_INFO_LOG_LENGTH, &infoLength);

			std::vector<GLchar> infoLog(infoLength);
			glGetProgramInfoLog(m_rendererId, infoLength, &infoLength, infoLog.data());

			glDeleteProgram(m_rendererId);

			for (auto shaderData : shaderDatas)
				glDeleteShader(shaderData.id);

			std::cerr << infoLog.data();
			GT_ASSERT(false, "Shader link failed.");
			return;
		}

		for (auto shaderData : shaderDatas)
			glDetachShader(m_rendererId, shaderData.id);
	}

	OpenGLShader::~OpenGLShader() { glDeleteProgram(m_rendererId); }

	void OpenGLShader::bind() { glUseProgram(m_rendererId); }
	void OpenGLShader::unbind() { glUseProgram(0); }

	int32_t OpenGLShader::findUniform(const std::string& a_name) {
		const auto& iter = m_uniforms.find(a_name);

		if (iter == m_uniforms.end()) {
			const int32_t location = glGetUniformLocation(m_rendererId, a_name.c_str());
			GT_ASSERT(location != -1, "Cannot find uniform in shader.");
			m_uniforms.emplace(a_name, location);
			return location;
		}

		return iter->second;
	}

	void OpenGLShader::setInt(const std::string& a_name, const int a_value) {
		glUniform1i(findUniform(a_name), a_value);
	}

	void OpenGLShader::setMatrix4x4(const std::string& a_name, const Matrix4x4& a_value) {
		glUniformMatrix4fv(findUniform(a_name), 1, GL_FALSE, a_value.valuePtr());
	}
}
