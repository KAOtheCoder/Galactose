#include "CameraController.h"

#include <Core/Events/KeyEvent.h>
#include <Core/Events/MouseEvent.h>
#include <Scene/Components/Transform.h>

using namespace Galactose;

namespace GalactoseEditor {
	void CameraController::onEvent(const std::shared_ptr<Event>& a_event) {
		//std::cout << a_event->toString() << std::endl;
		auto transform = getTransform();

		switch (a_event->type()) {
		case Event::KeyPress: {
			const auto key = static_cast<KeyEvent*>(a_event.get())->key();
			const float speed = 10 * time().deltaTime();
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
				const float speed = 2 * time().deltaTime();
				transform->setRotation(Quaternion::fromEulerDegrees(transform->rotation().eulerDegrees() + (Vector3(move.y, move.x, 0) * speed)));
			}
			break;
		default:
			return;
		}

		a_event->setHandled();
	}
}