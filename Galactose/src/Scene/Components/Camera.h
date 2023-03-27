#pragma once

#include "Component.h"
#include "Math/Math.h"

namespace Galactose {
	class Camera final : public Component {
		GT_COMPONENT(Camera)

	public:
		Camera() = default;
		~Camera() override;

		void start() override;

		float fov() const { return m_fov; }
		void setFOV(const float a_fov) { m_fov = a_fov; }

		float aspectRatio() const { return m_aspectRatio; }
		void setAspectRatio(const float a_aspectRatio) { m_aspectRatio = a_aspectRatio; }

		float nearClip() const { return m_nearClip; }
		void setNearClip(const float a_nearClip) { m_nearClip = a_nearClip; }

		float farClip() const { return m_farClip; }
		void setFarClip(const float a_farClip) { m_farClip = a_farClip; }

		Matrix4x4 viewProjectionMatrix() const;

	protected:
		void saveContent(YAML::Emitter& emitter) const override;
		bool loadContent(const YAML::Node& node) override;

	private:
		float m_fov = 60;
		float m_aspectRatio = 16.f / 9.f;
		float m_nearClip = 0.1f;
		float m_farClip = 5000.f;
	};
}
