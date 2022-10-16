#include "EditorLayer.h"

#include <Core/Events/KeyEvent.h>
#include <Core/Events/MouseEvent.h>
#include <Renderer/Renderer.h>
#include <Renderer/Camera.h>
#include <Renderer/Shader.h>

using namespace Galactose;

namespace GalactoseEditor {
	EditorLayer::EditorLayer() :
		m_texture(Texture::create("PandaLogo.png")),
		m_direction(0, 0, 1),
		m_up(0, 1, 0)
	{}

	void EditorLayer::onUpdate() {
		const auto& projection = m_camera.projectionMatrix();
		const Matrix4x4& view = glm::lookAt(m_position, m_position + m_direction, m_up);
		const Matrix4x4 model(1.f);
		const Matrix4x4 mvp = projection * view * model;

		const auto& renderer = Renderer::renderer();
		renderer->clear();
		renderer->shader()->setMatrix4x4("u_mvp", mvp);
		renderer->drawSprite(Vector3(0, 0, 1), { 1.0f, 1.0f }, m_texture);
	}

	void EditorLayer::onEvent(const std::shared_ptr<Event>& a_event) {
		std::cout << a_event->toString() << std::endl;

		switch (a_event->type()) {
		case Event::KeyPress:
		case Event::KeyRepeat: {
			const auto key = static_cast<KeyEvent*>(a_event.get())->key();
			const float speed = 0.05f;
			const auto& right = glm::cross(m_direction, m_up);

			switch (key)
			{
			case KeyEvent::KeyS: m_position -= m_direction * speed;
				break;
			case KeyEvent::KeyW: m_position += m_direction * speed;
				break;
			case KeyEvent::KeyA: m_position -= right * speed;
				break;
			case KeyEvent::KeyD: m_position += right * speed;
				break;
			case KeyEvent::KeyQ: m_position -= m_up * speed;
				break;
			case KeyEvent::KeyE: m_position += m_up * speed;
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
				 const auto& right = glm::cross(m_direction, m_up);
				 rotationMatrix = glm::rotate(rotationMatrix, Math::degreesToRadians(move.x * speed), m_up);
				 rotationMatrix = glm::rotate(rotationMatrix, Math::degreesToRadians(move.y * speed), right);
				 m_direction = rotationMatrix * Vector4(m_direction, 1);
			}
			break;
		}

		a_event->setHandled();
	}
}
