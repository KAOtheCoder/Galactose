#include "SceneViewport.h"
#include "EditorSceneData.h"

#include <Renderer/Framebuffer.h>
#include <Core/Events/KeyEvent.h>
#include <Core/Events/MouseEvent.h>
#include <Scene/Components/Transform.h>
#include <Scene/Components/Camera.h>

#include <imgui.h>

using namespace Galactose;

namespace GalactoseEditor {
	SceneViewport::SceneViewport(const std::shared_ptr<EditorSceneData>& a_sceneData) :
		Panel("Scene"),
		m_sceneData(a_sceneData),
		m_framebuffer(Framebuffer::create(1, 1, { Texture::RGBA8, Texture::Depth24Stencil8 })),
		m_cameraEntity(Entity::create(&m_privateScene))
	{
		setPadding({ 0, 0 });

		m_cameraEntity->addComponent<Camera>();
		m_cameraEntity->getTransform()->setPosition({ 0, 0, 9 });
	}

	void SceneViewport::onUpdate() {
		const auto& scene = m_sceneData->scene();
		if (scene) {
			const auto& viewportSize = ImGui::GetContentRegionAvail();
			if (viewportSize.x >= 1 && viewportSize.y >= 1) {
				auto scene = m_sceneData->scene();
				if (scene) {
					m_framebuffer->resize(int32_t(viewportSize.x), int32_t(viewportSize.y));
					auto* camera = m_cameraEntity->getComponent<Camera>();
					camera->setAspectRatio(viewportSize.x / viewportSize.y);

					m_framebuffer->bind();
					scene->render(camera);
					m_framebuffer->unbind();
				}

				ImGui::Image((void*)(intptr_t)m_framebuffer->texture(0)->rendererId(), viewportSize, { 0, 1 }, { 1, 0 });
			}
		}
	}

	void SceneViewport::onEvent(const std::shared_ptr<Event>& a_event) {
		//std::cout << a_event->toString() << std::endl;
		auto transform = m_cameraEntity->getTransform();

		switch (a_event->type()) {
		case Event::KeyPress: {
			const auto key = static_cast<KeyEvent*>(a_event.get())->key();
			const float speed = 0.05f;
			Vector3 direction;

			switch (key)
			{
			case KeyEvent::KeyS: direction = transform->forward();
				break;
			case KeyEvent::KeyW: direction = -transform->forward();
				break;
			case KeyEvent::KeyA: direction = -transform->right();
				break;
			case KeyEvent::KeyD: direction = transform->right();
				break;
			case KeyEvent::KeyQ: direction = -transform->up();
				break;
			case KeyEvent::KeyE: direction = transform->up();
				break;
			}

			transform->setPosition(transform->position() + (direction * speed));
			break;
		}
		case Event::MousePress: m_rotate = true;
			m_cursorPos = static_cast<MouseEvent*>(a_event.get())->cursorPosition();
			break;
		case Event::MouseRelease: m_rotate = false;
			break;
		case Event::MouseMove:
			if (m_rotate) {
				const auto& cursorPos = static_cast<MouseEvent*>(a_event.get())->cursorPosition();
				const auto& move = cursorPos - m_cursorPos;
				m_cursorPos = cursorPos;
				const float speed = 0.1f;
				transform->setRotation(Quaternion::fromEulerDegrees(transform->rotation().eulerDegrees() + (Vector3(move.y, move.x, 0) * speed)));
			}
			break;
		}

		a_event->setHandled();
	}
}
