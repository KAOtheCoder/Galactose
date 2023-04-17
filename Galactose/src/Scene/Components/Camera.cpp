#include "Camera.h"
#include "Transform.h"
#include "Scene/Serialize.h"

namespace Galactose {
	Camera::~Camera() {
		auto scene = entity()->scene();
		if (scene->mainCamera() == this)
			scene->setMainCamera(nullptr);
	}

	void Camera::start() {
		auto scene = entity()->scene();
		if (!scene->mainCamera())
			scene->setMainCamera(this);
	}

	Matrix4x4 Camera::viewProjectionMatrix() const {
		auto transform = getTransform();
		const auto& eye = transform->position();
		const auto& view = glm::lookAt(eye, eye - transform->forward(), transform->up());
		const auto& projection = glm::perspective(Math::degreesToRadians(m_fov), m_aspectRatio, m_nearClip, m_farClip);
		return projection * view;
	}

	void Camera::saveContent(YAML::Emitter& a_out) const {
		a_out << YAML::Key << "fov" << YAML::Value << m_fov
			<< YAML::Key << "nearClip" << YAML::Value << m_nearClip
			<< YAML::Key << "farClip" << YAML::Value << m_farClip
			<< YAML::Key << "backgroundColor" << YAML::Value << m_backgroundColor;
	}

	bool Camera::loadContent(const YAML::Node& a_node) {
		setFOV(a_node["fov"].as<float>(m_fov));
		setNearClip(a_node["nearClip"].as<float>(m_nearClip));
		setFarClip(a_node["farClip"].as<float>(m_farClip));
		setBackgroundColor(a_node["backgroundColor"].as<Vector3>(m_backgroundColor));
		return true;
	}
}
