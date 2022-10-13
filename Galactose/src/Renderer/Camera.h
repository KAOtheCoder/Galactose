#pragma once

#include "GalactosePCH.h"
#include "Core/Math.h"

namespace Galactose {
	class Camera {
	public:
		Camera() = default;
		virtual ~Camera() = default;

		float fov() const { return m_fov; }
		void setFOV(const float fov);

		float aspectRatio() const { return m_aspectRatio; }
		void setAspectRatio(const float aspectRatio);

		float nearClip() const { return m_nearClip; }
		void setNearClip(const float nearClip);

		float farClip() const { return m_farClip; }
		void setFarClip(const float farClip);

		Matrix4x4 projectionMatrix() const { return glm::perspective(m_fov, m_aspectRatio, m_nearClip, m_farClip); }

	private:
		float m_fov = Math::degreesToRadians(45);
		float m_aspectRatio = 16.f / 9.f;
		float m_nearClip = 0.1f;
		float m_farClip = 1000.f;
	};
}
