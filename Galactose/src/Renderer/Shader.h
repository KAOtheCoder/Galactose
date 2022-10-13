#pragma once

#include "Core/Math.h"

namespace Galactose {
	class Shader {
	public:
		static std::shared_ptr<Shader> create();
		virtual ~Shader() = default;

		virtual void bind() = 0;
		virtual void unbind() = 0;

		virtual void setMatrix4x4(const std::string& name, const Matrix4x4& value) = 0;

	protected:
		std::unordered_map<std::string, int32_t> m_uniforms;
	};
}