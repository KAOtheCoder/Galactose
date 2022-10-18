#pragma once

#include "GalactosePCH.h"
#include "Math.h"

namespace Galactose {
	class Transform {
	public:
		Transform() = default;

		Vector3 position() const { return m_position; };
		void setPosition(const Vector3& a_position) { m_position = a_position; }

		Quaternion rotation() const { return m_rotation; }
		void setRotation(const Quaternion& a_rotation) { m_rotation = a_rotation; }

		Vector3 scale() const { return m_scale; }
		void setScale(const Vector3& a_scale) { m_scale = a_scale; }

		Matrix4x4 matrix() const;

	private:
		Vector3 m_position = { 0, 0, 0 };
		Quaternion m_rotation;
		Vector3 m_scale = { 1, 1, 1 };
	};
}
