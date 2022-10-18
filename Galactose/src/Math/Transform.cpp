#include "GalactosePCH.h"
#include "Transform.h"

#include <glm/gtx/transform.hpp>

namespace Galactose {
	Matrix4x4 Transform::matrix() const {
		return glm::translate(m_position) * m_rotation.toMatrix() * glm::scale(m_scale);
	}
}
