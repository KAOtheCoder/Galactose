#include "GalactosePCH.h"

#include "Camera.h"

namespace Galactose {
	Camera::Camera() :
		m_view(1)
	{}

	Matrix4x4 Camera::projectionMatrix() const {
		return glm::perspective(m_fov, m_aspectRatio, m_nearClip, m_farClip);
	}

	void Camera::setView(const Vector3& a_position, const Vector3& a_direction, const Vector3& a_up) {
		m_view = glm::lookAt(a_position, a_position + a_direction, a_up);
	}
}
