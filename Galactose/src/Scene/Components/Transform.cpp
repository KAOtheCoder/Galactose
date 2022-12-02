#include "Transform.h"

#include <glm/gtx/transform.hpp>

namespace Galactose {
	// TO DO: cache matrices
	Matrix4x4 Transform::matrix() const {
		return glm::translate(m_position) * m_rotation.toMatrix() * glm::scale(m_scale);
	}

	Matrix4x4 Transform::worldMatrix() const {
		auto parentEntity = entity()->parent();
		if (parentEntity)
			return parentEntity->getTransform()->worldMatrix() * matrix();

		return Matrix4x4(1);
	}
}
