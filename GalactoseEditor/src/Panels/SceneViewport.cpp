#include "SceneViewport.h"
#include "CameraController.h"
#include "EditorContext.h"

#include <Galactose/Scene/Components/Transform.h>
#include <Galactose/Scene/Components/Camera.h>
#include <Galactose/Core/Property.h>

#include <imgui.h>

using namespace Galactose;

namespace GalactoseEditor {
	SceneViewport::SceneViewport(const std::shared_ptr<EditorContext>& a_sceneContext) :
		Viewport("Scene", a_sceneContext),
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
		if (m_usingManipulator)
			a_event->setHandled();
		else
			m_privateScene.processEvent(a_event);
	}

	void SceneViewport::onUpdate() {
		m_privateScene.time().tick();
		Viewport::onUpdate();

		m_usingManipulator = false;

		const auto selectedEntity = m_sceneContext->selectedEntity();
		if (selectedEntity) {
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			const auto& viewportSize = ImGui::GetWindowSize();

			if (viewportSize.x >= 1 || viewportSize.y >= 1) {
				const auto& panelPos = ImGui::GetWindowPos();
				ImGuizmo::SetRect(panelPos.x, panelPos.y, viewportSize.x, viewportSize.y);

				const auto editorCamera = m_cameraEntity->getComponent<Camera>();
				const auto& view = editorCamera->viewMatrix();
				const auto& projection = editorCamera->projectionMatrix();
				auto transform = selectedEntity->getTransform();
				auto matrix = transform->localMatrix();
				const auto operation = m_sceneContext->manipulatorOperation();

				if (ImGuizmo::Manipulate(view.data(), projection.data(), operation, ImGuizmo::LOCAL, matrix.data())) {
					switch (operation) {
					case ImGuizmo::TRANSLATE: {
						Vector3 position;
						matrix.decomposeAffine(&position);
						transform->setLocalPosition(position);
						break;
					}
					case ImGuizmo::ROTATE: {
						Quaternion rotation;
						matrix.decomposeAffine(nullptr, &rotation);
						transform->setLocalRotation(rotation);
						break;
					}
					case ImGuizmo::SCALE: {
						// TODO: fix scale gizmo
						Vector3 scale;
						matrix.decomposeAffine(nullptr, nullptr, &scale);
						transform->setLocalScale(scale);
						break;
					}
					default:
						GT_ASSERT(false, "Unknown operation: " + std::to_string(operation));
					}
				}

				m_usingManipulator = ImGuizmo::IsUsing();
			}
		}
	}
}
