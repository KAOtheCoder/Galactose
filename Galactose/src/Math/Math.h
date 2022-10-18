#pragma once

#define GLM_FORCE_CTOR_INIT

#include <glm/glm.hpp>
#include "glm/gtx/string_cast.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

#define GT_GLM_EXTEND_IMP(_class, _baseClass) \
	public:\
		using glm::_baseClass::_baseClass;\
		std::string toString() const { return glm::to_string(base()); }\
		const float* valuePtr() const { return glm::value_ptr(base()); }\
	private:\
		const glm::_baseClass& base() const { return *static_cast<const glm::_baseClass*>(this); }\

#define GT_GLM_EXTEND(_class, _baseClass) class _class : public glm::_baseClass {\
	GT_GLM_EXTEND_IMP(_class, _baseClass) \
}

namespace Galactose {
	GT_GLM_EXTEND(Vector2, vec2);
	GT_GLM_EXTEND(Vector3, vec3);
	GT_GLM_EXTEND(Vector4, vec4);
	GT_GLM_EXTEND(Matrix4x4, mat4);
	
	class Quaternion : public glm::quat {
		GT_GLM_EXTEND_IMP(Quaternion, quat)

	public:
		Matrix4x4 toMatrix() const { return glm::toMat4(*this); }
	};

	namespace Math {
		static float degreesToRadians(const float a_degrees) { return glm::radians(a_degrees); }
	}
}

#undef GT_GLM_EXTEND_IMP
#undef GT_GLM_EXTEND
