#pragma once

#include <glm/glm.hpp>
#include "glm/gtx/string_cast.hpp"

#define GT_GLM_EXTEND_IMP(_class, _baseClass) class _class : public glm::_baseClass {\
	public:\
		using glm::_baseClass::vec;\
		std::string toString() const { return glm::to_string(glm::_baseClass(*this)); }\
	}

namespace Galactose {
	GT_GLM_EXTEND_IMP(Vector2, vec2);
	GT_GLM_EXTEND_IMP(Vector3, vec3);
	GT_GLM_EXTEND_IMP(Vector4, vec4);
}
