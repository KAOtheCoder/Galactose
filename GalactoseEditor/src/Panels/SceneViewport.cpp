#include "SceneViewport.h"
#include "CameraController.h"

#include <Scene/Components/Transform.h>
#include <Scene/Components/Camera.h>

#include <imgui.h>

using namespace Galactose;

namespace GalactoseEditor {
	SceneViewport::SceneViewport(const std::shared_ptr<EditorSceneData>& a_sceneData) :
		Viewport("Scene", a_sceneData),
		m_privateScene("EditorPrivate"),
		m_cameraEntity(Entity::create(&m_privateScene))
	{
		m_cameraEntity->setName("EditorCamera");
		m_cameraEntity->getTransform()->setPosition({ 0, 0, 9 });

		m_cameraEntity->addComponent<Camera>();
		m_cameraEntity->addComponent<CameraController>();
	}

	Camera* SceneViewport::getCamera() const { return m_cameraEntity->getComponent<Camera>(); }

	void SceneViewport::onEvent(const std::shared_ptr<Event>& a_event) {
		m_privateScene.processEvent(a_event);
	}
}
