#include "SceneViewport.h"
#include "EditorSceneData.h"

#include <Renderer/Framebuffer.h>
#include <Core/Events/KeyEvent.h>
#include <Core/Events/MouseEvent.h>
#include <Renderer/Renderer.h>

#include <imgui.h>

using namespace Galactose;

namespace GalactoseEditor {
	SceneViewport::SceneViewport(const std::shared_ptr<EditorSceneData>& a_sceneData) :
		Panel("Scene"),
		m_sceneData(a_sceneData),
		m_framebuffer(Framebuffer::create(1, 1, { Texture::RGBA8, Texture::Depth24Stencil8 })),
		m_cameraPosition(0, 0, -1),
		m_cameraDirection(0, 0, 1),
		m_up(0, 1, 0)
	{
		setPadding({ 0, 0 });
	}

	void SceneViewport::onUpdate() {
		const auto& scene = m_sceneData->scene();
		if (scene) {
			const auto& viewportSize = ImGui::GetContentRegionAvail();
			if (viewportSize.x >= 1 && viewportSize.y >= 1) {
				m_framebuffer->resize(int32_t(viewportSize.x), int32_t(viewportSize.y));
				m_camera.setAspectRatio(viewportSize.x / viewportSize.y);

				m_camera.setView(m_cameraPosition, m_cameraDirection, m_up);
				const auto& renderer = Renderer::renderer();
				m_framebuffer->bind();
				renderer->clear();
				renderer->setViewProjection(m_camera);
				//renderer->drawQuad({ -1, 0, 1 }, { 1.0f, 1.0f }, m_texture);
				renderer->drawQuad({ 0, 0, 1 }, { 1.0f, 1.0f }, { 0.8f, 0.1f, 0.1f, 0.8 });
				m_framebuffer->unbind();

				ImGui::Image(ImTextureID(m_framebuffer->texture(0)->rendererId()), viewportSize, { 0, 1 }, { 1, 0 });
			}
		}
	}

	void SceneViewport::onEvent(const std::shared_ptr<Event>& a_event) {
		std::cout << a_event->toString() << std::endl;

		switch (a_event->type()) {
		case Event::KeyPress: {
			const auto key = static_cast<KeyEvent*>(a_event.get())->key();
			const float speed = 0.05f;
			const auto& right = glm::cross(m_cameraDirection, m_up);

			switch (key)
			{
			case KeyEvent::KeyS: m_cameraPosition -= m_cameraDirection * speed;
				break;
			case KeyEvent::KeyW: m_cameraPosition += m_cameraDirection * speed;
				break;
			case KeyEvent::KeyA: m_cameraPosition -= right * speed;
				break;
			case KeyEvent::KeyD: m_cameraPosition += right * speed;
				break;
			case KeyEvent::KeyQ: m_cameraPosition -= m_up * speed;
				break;
			case KeyEvent::KeyE: m_cameraPosition += m_up * speed;
				break;
			}
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
				Matrix4x4 rotationMatrix(1);
				const auto& right = Vector3::cross(m_cameraDirection, m_up);
				rotationMatrix = glm::rotate(rotationMatrix, Math::degreesToRadians(move.x * speed), m_up);
				rotationMatrix = glm::rotate(rotationMatrix, Math::degreesToRadians(move.y * speed), right);
				m_cameraDirection = rotationMatrix * Vector4(m_cameraDirection, 1);
			}
			break;
		}

		a_event->setHandled();
	}
}
