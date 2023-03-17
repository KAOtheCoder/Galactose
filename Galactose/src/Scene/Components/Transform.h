#pragma once

#include "Component.h"
#include "Math/Math.h"

namespace Galactose {
	class Transform final : public Component {
		GT_COMPONENT(Transform)
	
	public:
		Transform() = default;

		Vector3 localPosition() const { return m_localPosition; };
		void setLocalPosition(const Vector3& position);

		Quaternion localRotation() const { return m_localRotation; }
		void setLocalRotation(const Quaternion& rotation);

		Vector3 localScale() const { return m_localScale; }
		void setLocalScale(const Vector3& scale);

		Vector3 position() const;
		void setPosition(const Vector3& position);

		Quaternion rotation() const;
		void setRotation(const Quaternion& rotation);

		Vector3 lossyScale() const;

		Matrix4x4 localToWorldMatrix() const;
		Matrix4x4 worldToLocalMatrix() const { return localToWorldMatrix().affineInverse(); }

		Vector3 right() const;
		Vector3 up() const;
		Vector3 forward() const;

		Vector3 transformPoint(const Vector3& point) const;
		Vector3 transformVector(const Vector3& vector) const;

	protected:
		void saveContent(YAML::Emitter& out) const override;
		bool loadContent(const YAML::Node& node) override;

	private:
		static void decomposeAffine(const Matrix4x4& transform, Vector3& position, Quaternion& rotation, Vector3& scale);

		Matrix4x4 localMatrix() const;
		void updateLocalToWorldMatrix() const;

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
