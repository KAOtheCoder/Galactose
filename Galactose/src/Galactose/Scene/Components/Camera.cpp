#include "Camera.h"
#include "Transform.h"
#include "Galactose/Serialization/OutSerializer.h"
#include "Galactose/Serialization/NodeSerializer.h"

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

	Matrix4x4 Camera::viewMatrix() const {
		auto transform = getTransform();
		const auto& eye = transform->position();
		return glm::lookAt(eye, eye - transform->forward(), transform->up());
	}

	Matrix4x4 Camera::projectionMatrix() const {
		return glm::perspective(Math::degreesToRadians(m_fov), m_aspectRatio, m_nearClip, m_farClip);
	}

	void Camera::saveContent(OutSerializer& a_out) const {
		a_out << OutSerializer::Key << "fov" << OutSerializer::Value << m_fov
			<< OutSerializer::Key << "nearClip" << OutSerializer::Value << m_nearClip
			<< OutSerializer::Key << "farClip" << OutSerializer::Value << m_farClip
			<< OutSerializer::Key << "backgroundColor" << OutSerializer::Value << m_backgroundColor;
	}

	bool Camera::loadContent(const NodeSerializer& a_node) {
		setFOV(a_node["fov"].as(m_fov));
		setNearClip(a_node["nearClip"].as(m_nearClip));
		setFarClip(a_node["farClip"].as(m_farClip));
		setBackgroundColor(a_node["backgroundColor"].as(m_backgroundColor));
		return true;
	}
}
