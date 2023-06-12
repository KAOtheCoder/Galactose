#pragma once

#include "Component.h"
#include "Galactose/Math/Math.h"

namespace Galactose {
	class Transform final : public Component {
		GT_COMPONENT(Transform)
	
	public:
		Transform() = default;

		Vector3 localPosition() const { return m_localPosition; };
		GT_API void setLocalPosition(const Vector3& position);

		Quaternion localRotation() const { return m_localRotation; }
		GT_API void setLocalRotation(const Quaternion& rotation);

		Vector3 localScale() const { return m_localScale; }
		GT_API void setLocalScale(const Vector3& scale);

		GT_API Vector3 position() const;
		GT_API void setPosition(const Vector3& position);

		GT_API Quaternion rotation() const;
		GT_API void setRotation(const Quaternion& rotation);

		Vector3 lossyScale() const;

		GT_API Matrix4x4 localMatrix() const;
		Matrix4x4 localToWorldMatrix() const;
		Matrix4x4 worldToLocalMatrix() const { return localToWorldMatrix().affineInverse(); }

		GT_API Vector3 right() const;
		GT_API Vector3 up() const;
		GT_API Vector3 forward() const;

		Vector3 transformPoint(const Vector3& point) const;
		Vector3 transformVector(const Vector3& vector) const;

	protected:
		GT_API void saveContent(OutSerializer& out) const override;
		GT_API bool loadContent(const YAML::Node& node) override;

	private:
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
