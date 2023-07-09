#include "Transform.h"
#include "Galactose/Serialization/OutSerializer.h"
#include "Galactose/Serialization/NodeSerializer.h"

namespace Galactose {
	Matrix4x4 Transform::localMatrix() const {
		auto adjustedScale = m_localScale;

		for (int i = 0; i < 3; ++i) // avoid zero
			if (adjustedScale[i] == 0)
				adjustedScale[i] = glm::epsilon<float>();

		return Matrix4x4::transform(m_localPosition, m_localRotation, adjustedScale);
	}

	void Transform::updateLocalToWorldMatrix() const {
		const auto parentEntity = entity()->parent();
		const auto parent = parentEntity ? parentEntity->getTransform() : nullptr;
		if (!m_dirty && (!parent || !parent->m_dirty))
			return;

		if (parent) {
			m_worldMatrix = parent->localToWorldMatrix() * localMatrix();
			m_worldMatrix.decomposeAffine(&m_position, &m_rotation, &m_scale);
		}
		else {
			m_worldMatrix = localMatrix();
			m_position = m_localPosition;
			m_rotation = m_localRotation;
			m_scale = m_localScale;
		}

		m_dirty = false;
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

		if (!parentEntity) {
			setLocalPosition(a_position);
			return;
		}

		auto worldMatrix = localToWorldMatrix();
		worldMatrix.setColumn(3, Vector4(a_position, 1));
		const Matrix4x4 localMatrix = parentEntity->getTransform()->worldToLocalMatrix() * worldMatrix;
		setLocalPosition(localMatrix.column(3));
	}

	Quaternion Transform::rotation() const {
		updateLocalToWorldMatrix();
		return m_rotation;
	}

	void Transform::setRotation(const Quaternion& a_rotation) {
		const auto parentEntity = entity()->parent();

		if (!parentEntity) {
			setLocalRotation(a_rotation);
			return;
		}

		auto worldMatrix = localToWorldMatrix();
		Vector3 position, scale;
		worldMatrix.decomposeAffine(&position, nullptr, &scale);
		worldMatrix = Matrix4x4::transform(position, a_rotation, scale);
		const Matrix4x4 localMatrix = parentEntity->getTransform()->worldToLocalMatrix() * worldMatrix;
		Quaternion localRotation;
		localMatrix.decomposeAffine(nullptr, &localRotation, nullptr);
		setLocalRotation(localRotation);
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

	Vector3 Transform::right() const {
		return transformVector({ 1, 0, 0 }).normalized();
	}

	Vector3 Transform::up() const {
		return transformVector({ 0, 1, 0 }).normalized();
	}

	Vector3 Transform::forward() const {
		return transformVector({ 0, 0, 1 }).normalized();
	}

	Vector3 Transform::transformPoint(const Vector3& point) const {
		return localToWorldMatrix() * Vector4(point, 1);
	}

	Vector3 Transform::transformVector(const Vector3& vector) const {
		return transformPoint(vector) - transformPoint({ 0, 0, 0 });
	}

	void Transform::saveContent(OutSerializer& a_out) const {
		a_out << OutSerializer::Key << "localPosition" << OutSerializer::Value << m_localPosition
			<< OutSerializer::Key << "localRotation" << OutSerializer::Value << m_localRotation
			<< OutSerializer::Key << "localScale" << OutSerializer::Value << m_localScale;
	}

	bool Transform::loadContent(const NodeSerializer& a_node) {
		m_localPosition = a_node["localPosition"].as(m_localPosition);
		m_localRotation = a_node["localRotation"].as(m_localRotation);
		m_localScale = a_node["localScale"].as(m_localScale);
		m_dirty = true;

		return true;
	}
}
