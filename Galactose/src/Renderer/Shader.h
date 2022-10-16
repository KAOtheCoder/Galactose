#pragma once

namespace Galactose {
	class Matrix4x4;

	class Shader {
	public:
		static std::shared_ptr<Shader> create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);

		virtual ~Shader() = default;

		std::string name() const { return m_name; }

		virtual void bind() = 0;
		virtual void unbind() = 0;

		virtual void setInt(const std::string& name, const int value) = 0;
		virtual void setMatrix4x4(const std::string& name, const Matrix4x4& value) = 0;

	protected:
		Shader(const std::string& a_name) : m_name(a_name) {}

		std::string m_name;
		std::unordered_map<std::string, int32_t> m_uniforms;
	};
}
