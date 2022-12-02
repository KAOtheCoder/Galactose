#pragma once

#include "Math/Math.h"

namespace Galactose {
	class Camera {
	public:
		Camera();
		virtual ~Camera() = default;

		float fov() const { return m_fov; }
		void setFOV(const float a_fov) { m_fov = a_fov; }

		float aspectRatio() const { return m_aspectRatio; }
		void setAspectRatio(const float a_aspectRatio) { m_aspectRatio = a_aspectRatio; }

		float nearClip() const { return m_nearClip; }
		void setNearClip(const float a_nearClip) { m_nearClip = a_nearClip; }

		float farClip() const { return m_farClip; }
		void setFarClip(const float a_farClip) { m_farClip = a_farClip; }

		Matrix4x4 projectionMatrix() const;

		Matrix4x4 viewMatrix() const { return m_view; }
		void setView(const Vector3& position, const Vector3& direction, const Vector3& up);

	private:
		float m_fov = Math::degreesToRadians(60);
		float m_aspectRatio = 16.f / 9.f;
		float m_nearClip = 0.1f;
		float m_farClip = 1000.f;
		Matrix4x4 m_view;
	};
}
