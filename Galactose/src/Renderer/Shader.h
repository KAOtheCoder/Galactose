#pragma once

#include <memory>
#include <unordered_map>
#include <string>

namespace Galactose {
	class Matrix4x4;
	class Vector2;
	class Vector3;
	class Vector4;

	class Shader {
	public:
		static std::shared_ptr<Shader> create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);

		virtual ~Shader() = default;

		std::string name() const { return m_name; }

		virtual void bind() = 0;
		virtual void unbind() = 0;

		virtual void setInt(const std::string& name, const int value) = 0;
		virtual void setVector2(const std::string& name, const Vector2& value) = 0;
		virtual void setVector3(const std::string& name, const Vector3& value) = 0;
		virtual void setVector4(const std::string& name, const Vector4& value) = 0;
		virtual void setMatrix4x4(const std::string& name, const Matrix4x4& value) = 0;

		int32_t getUniform(const std::string& name) const;

	protected:
		Shader(const std::string& a_name) : m_name(a_name) {}

		std::string m_name;
		std::unordered_map<std::string, int32_t> m_uniforms;
	};
}
