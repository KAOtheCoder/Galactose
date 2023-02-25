#include "Transform.h"
#include "Core/Global.h"
#include "Scene/Serialize.h"

namespace Galactose {
	Matrix4x4 Transform::localMatrix() const {
		auto adjustedScale = m_localScale;

		for (int i = 0; i < 3; ++i) // avoid zero
			if (adjustedScale[i] == 0)
				adjustedScale[i] = glm::epsilon<float>();

		return Matrix4x4::translate(m_localPosition) * m_localRotation.toMatrix() * Matrix4x4::scale(adjustedScale);
	}

	void Transform::updateLocalToWorldMatrix() const {
		const auto parentEntity = entity()->parent();
		const auto parent = parentEntity ? parentEntity->getTransform() : nullptr;
		if (!m_dirty && (!parent || !parent->m_dirty))
			return;

		if (parent) {
			m_worldMatrix = parent->localToWorldMatrix() * localMatrix();

			decomposeAffine(m_worldMatrix, m_position, m_rotation, m_scale);

			m_position = m_worldMatrix[3];
			m_rotation = parent->m_rotation * m_localRotation;
			m_scale = parent->m_worldMatrix * Vector4(1, 1, 1, 1) - parent->m_worldMatrix * Vector4(0, 0, 0, 1);
		}
		else {
			m_worldMatrix = localMatrix();
			m_position = m_localPosition;
			m_rotation = m_localRotation;
			m_scale = m_localScale;
		}

		m_dirty = false;
	}

	// Inspired from cglm::glm_decompose
	void Transform::decomposeAffine(const Matrix4x4& transform, Vector3& a_position, Quaternion& a_rotation, Vector3& a_scale) {
		a_position = transform[3];

		a_scale = { transform[0].length(), transform[1].length(), transform[2].length() };

		Matrix4x4 rotation(transform); // we only need 3x3 matrix, maybe change that later
		//rotation[3] = { 0, 0, 0, 1 };

		for (int i = 0; i < 3; ++i) // avoid divide by zero
			rotation[i] /= a_scale[i] == 0 ? glm::epsilon<float>() : a_scale[i];

		// check for a coordinate system flip.If the determinant
		// is - 1, then negate the matrixand the scaling factors.
		const auto& v = Vector3::cross(transform[0], transform[1]);
		if (Vector3::dot(v, transform[2]) < 0) {
			rotation[0] = -rotation[0];
			rotation[1] = -rotation[1];
			rotation[2] = -rotation[2];
			a_scale = -a_scale;
		}

		a_rotation = rotation;
	}

	Matrix4x4 Transform::localToWorldMatrix() const {
		updateLocalToWorldMatrix();
		return m_worldMatrix;
	}

	Vector3 Transform::position() const {
		updateLocalToWorldMatrix();
		return m_position;
	}

	void Transform::setPosition(const Vector3& a_position) {
		const auto parentEntity = entity()->parent();

		setLocalPosition(parentEntity 
			? parentEntity->getTransform()->localToWorldMatrix().affineInverse() * Vector4(a_position, 1)
			: a_position
		);
	}

	Quaternion Transform::rotation() const {
		updateLocalToWorldMatrix();
		return m_rotation;
	}

	void Transform::setRotation(const Quaternion& a_rotation) {
		const auto parentEntity = entity()->parent();

		setLocalRotation(parentEntity
			? parentEntity->getTransform()->localToWorldMatrix().affineInverse() * a_rotation.toMatrix()
			: a_rotation
		);
	}

	Vector3 Transform::lossyScale() const {
		updateLocalToWorldMatrix();
		return m_scale;
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

	void Transform::save(YAML::Emitter& a_out) const {
		beginSave(a_out, GT_STRINGIFY(Transform));

		a_out << YAML::Key << "localPosition" << YAML::Value << m_localPosition
			<< YAML::Key << "localRotation" << YAML::Value << m_localRotation
			<< YAML::Key << "localScale" << YAML::Value << m_localScale;
		
		endSave(a_out);
	}
}
