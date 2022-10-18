#pragma once

#include "GalactosePCH.h"
#include "Core/Math.h"

namespace Galactose {
	class Camera {
	public:
		Camera();
		virtual ~Camera() = default;

		Matrix4x4 projectionMatrix() const { return m_projection; }
		void setProjection(const float fov, const float aspectRatio, const float nearClip, const float farClip);

		float fov() const { return m_fov; }
		void setFOV(const float fov);

		float aspectRatio() const { return m_aspectRatio; }
		void setAspectRatio(const float aspectRatio);

		float nearClip() const { return m_nearClip; }
		void setNearClip(const float nearClip);

		float farClip() const { return m_farClip; }
		void setFarClip(const float farClip);

		Matrix4x4 viewMatrix() const { return m_view; }
		void setView(const Vector3& position, const Vector3& direction, const Vector3& up);

	private:
		void updateProjection();

		Matrix4x4 m_projection;
		float m_fov = Math::degreesToRadians(60);
		float m_aspectRatio = 16.f / 9.f;
		float m_nearClip = 0.1f;
		float m_farClip = 1000.f;
		Matrix4x4 m_view;
	};
}
