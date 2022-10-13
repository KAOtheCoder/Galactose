#include "GalactosePCH.h"

#include "Camera.h"

namespace Galactose {
	void Camera::setFOV(const float a_fov) {
		m_fov = a_fov;
	}

	void Camera::setAspectRatio(const float a_aspectRatio) {
		m_aspectRatio = a_aspectRatio;
	}

	void Camera::setNearClip(const float a_nearClip) {
		m_nearClip = a_nearClip;
	}

	void Camera::setFarClip(const float a_farClip) {
		m_farClip = a_farClip;
	}
}