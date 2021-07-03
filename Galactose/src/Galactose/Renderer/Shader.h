#pragma once

namespace Galactose {
	class Shader {
	public:
		static std::shared_ptr<Shader> create();
		virtual ~Shader() = default;

		virtual void bind() = 0;
		virtual void unbind() = 0;
	};
}