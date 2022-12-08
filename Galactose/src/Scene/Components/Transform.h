#pragma once

#include "Component.h"
#include "Math/Math.h"

namespace Galactose {
	class Transform : public Component {
	public:
		Transform() = default;

		Vector3 localPosition() const { return m_localPosition; };
		void setLocalPosition(const Vector3& position);

		Quaternion localRotation() const { return m_localRotation; }
		void setLocalRotation(const Quaternion& rotation);

		Vector3 localScale() const { return m_localScale; }
		void setLocalScale(const Vector3& scale);

		Vector3 position() const;
		Quaternion rotation() const;

		Matrix4x4 localMatrix() const;
		Matrix4x4 worldMatrix() const;

	private:
		void updateWorldMatrix() const;

		Vector3 m_localPosition = { 0, 0, 0 };
		Quaternion m_localRotation;
		Vector3 m_localScale = { 1, 1, 1 };
		mutable bool m_dirty = true;
		mutable Matrix4x4 m_worldMatrix;
		mutable Vector3 m_position;
		mutable Quaternion m_rotation;
		mutable Vector3 m_scale;
	};
}
