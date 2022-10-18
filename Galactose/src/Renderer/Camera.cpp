#include "GalactosePCH.h"

#include "Camera.h"

namespace Galactose {
	Camera::Camera() {
		updateProjection();
	}

	void Camera::setFOV(const float a_fov) {
		m_fov = a_fov;
	}

	void Camera::setAspectRatio(const float a_aspectRatio) {
		m_aspectRatio = a_aspectRatio;
		updateProjection();
	}

	void Camera::setNearClip(const float a_nearClip) {
		m_nearClip = a_nearClip;
		updateProjection();
	}

	void Camera::setFarClip(const float a_farClip) {
		m_farClip = a_farClip;
		updateProjection();
	}

	void Camera::updateProjection() {
		m_projection = glm::perspective(m_fov, m_aspectRatio, m_nearClip, m_farClip);
	}

	void Camera::setProjection(const float a_fov, const float a_aspectRatio, const float a_nearClip, const float a_farClip) {
		m_fov = a_fov;
		m_aspectRatio = a_aspectRatio;
		m_nearClip = a_nearClip;
		m_farClip = a_farClip;

		updateProjection();
	}

	void Camera::setView(const Vector3& a_position, const Vector3& a_direction, const Vector3& a_up) {
		m_view = glm::lookAt(a_position, a_position + a_direction, a_up);
	}
}
