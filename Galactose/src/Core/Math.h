#pragma once

#include <glm/glm.hpp>
#include "glm/gtx/string_cast.hpp"
#include <glm/gtc/type_ptr.hpp>

#define GT_GLM_EXTEND_IMP(_class, _baseClass) class _class : public glm::_baseClass {\
	public:\
		using glm::_baseClass::_baseClass;\
		std::string toString() const { return glm::to_string(base()); }\
		const float* valuePtr() const { return glm::value_ptr(base()); }\
	private:\
		const glm::_baseClass& base() const { return *static_cast<const glm::_baseClass*>(this); }\
	}

namespace Galactose {
	GT_GLM_EXTEND_IMP(Vector2, vec2);
	GT_GLM_EXTEND_IMP(Vector3, vec3);
	GT_GLM_EXTEND_IMP(Vector4, vec4);
	GT_GLM_EXTEND_IMP(Matrix4x4, mat4);

	namespace Math {
		static float degreesToRadians(const float a_degrees) { return glm::radians(a_degrees); }
	}
}
