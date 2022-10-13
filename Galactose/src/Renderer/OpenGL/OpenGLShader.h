#pragma once

#include "GalactosePCH.h"
#include "Renderer/Shader.h"

#include <glad/glad.h>

namespace Galactose {
	class OpenGLShader : public Shader {
	public:
		OpenGLShader();
		~OpenGLShader() override { glDeleteProgram(m_rendererId); }

		void bind() override { glUseProgram(m_rendererId); }
		void unbind() override { glUseProgram(0); }

		void setMatrix4x4(const std::string& name, const Matrix4x4& value) override;

	private:
		int32_t findUniform(const std::string& name);

		uint32_t m_rendererId = 0;
	};
}