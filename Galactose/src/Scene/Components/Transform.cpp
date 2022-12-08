#include "Transform.h"
#include "Core/Global.h"

#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

namespace Galactose {
	// TO DO: cache matrices
	Matrix4x4 Transform::localMatrix() const {
		return glm::translate(m_localPosition) * m_localRotation.toMatrix() * glm::scale(m_localScale);
	}

	void Transform::updateWorldMatrix() const {
		auto parentEntity = entity()->parent();
		if (!m_dirty && (!parentEntity || !parentEntity->getTransform()->m_dirty))
			return;

		m_worldMatrix = parentEntity ? parentEntity->getTransform()->worldMatrix() * localMatrix() : localMatrix();

		Vector3 skew;
		Vector4 perspective;

		const bool ok = glm::decompose(m_worldMatrix, m_scale, m_rotation, m_position, skew, perspective);
		GT_ASSERT(ok, "Failed to decompose world matrix: " + m_worldMatrix.toString());

		m_dirty = false;
	}

	Matrix4x4 Transform::worldMatrix() const {
		updateWorldMatrix();
		return m_worldMatrix;
	}

	Vector3 Transform::position() const {
		updateWorldMatrix();
		return m_position;
	}

	Quaternion Transform::rotation() const {
		updateWorldMatrix();
		return m_rotation;
	}

	void Transform::setLocalPosition(const Vector3& a_position) {
		m_localPosition = a_position;
		m_dirty = true;
	}

	void Transform::setLocalRotation(const Quaternion& a_rotation) {
		m_localRotation = a_rotation;
		m_dirty = true;
	}

	void Transform::setLocalScale(const Vector3& a_scale) {
		m_localScale = a_scale;
		m_dirty = true;
	}
}
