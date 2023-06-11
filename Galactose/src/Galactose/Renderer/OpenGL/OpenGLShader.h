#pragma once

#include "Galactose/Renderer/Shader.h"

namespace Galactose {
	class OpenGLShader : public Shader {
	public:
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		~OpenGLShader() override;

		void bind() override;
		void unbind() override;

		void setInt(const std::string& name, const int value) override;
		void setVector2(const std::string& name, const Vector2& value) override;
		void setVector3(const std::string& name, const Vector3& value) override;
		void setVector4(const std::string& name, const Vector4& value) override;
		void setMatrix4x4(const std::string& name, const Matrix4x4& value) override;

	private:
		void mapUniforms();

		uint32_t m_rendererId = 0;
	};
}