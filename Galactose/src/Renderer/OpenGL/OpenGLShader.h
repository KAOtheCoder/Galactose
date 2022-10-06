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

	private:
		uint32_t m_rendererId = 0;
	};
}