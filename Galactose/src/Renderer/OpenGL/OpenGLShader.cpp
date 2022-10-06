#include "GalactosePCH.h"
#include "OpenGLShader.h"

namespace Galactose {
	OpenGLShader::OpenGLShader() {
		const std::string& vertexSource = R"(
			#version 330 core

			layout(location = 0) in vec3 i_position;
			
			out vec4 v_color;

			void main() {
				gl_Position = vec4(i_position, 1.0);
				v_color = vec4(i_position + 0.5, 1.0);
			}
		)";

		const std::string& pixelSource = R"(
			#version 330 core

			layout(location = 0) out vec4 o_color;

			in vec4 v_color;

			void main() {
				o_color = v_color;
			}
		)";

		struct ShaderData {
			GLuint id = 0;
			GLenum type;
			std::string source;
		};

		std::array<ShaderData, 2> shaderDatas = { {
			{0, GL_VERTEX_SHADER, vertexSource}, {0, GL_FRAGMENT_SHADER, pixelSource}
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

		for (auto shaderData : shaderDatas)
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
}