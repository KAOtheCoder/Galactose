#include "Camera.h"
#include "Transform.h"

namespace Galactose {
	Matrix4x4 Camera::viewProjectionMatrix() const {
		auto transform = getTransform();
		const auto& eye = transform->position();
		const auto& view = glm::lookAt(eye, eye - transform->forward(), transform->up());
		const auto& projection = glm::perspective(m_fov, m_aspectRatio, m_nearClip, m_farClip);
		return projection * view;
	}

	void Camera::saveContent(YAML::Emitter& a_out) const {
	}

	bool Camera::loadContent(const YAML::Node& a_node) {
		return true;
	}
}
