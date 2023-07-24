#include "CameraController.h"

#include <Galactose/Core/Events/KeyEvent.h>
#include <Galactose/Core/Events/MouseEvent.h>
#include <Galactose/Core/Window.h>
#include <Galactose/Scene/Components/Transform.h>

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

		case Event::MouseMove: {
			const auto mouseEvent = static_cast<MouseEvent*>(a_event.get());

			if (m_rotate && mouseEvent->window()->isMouseButtonPressed(MouseEvent::Left)) {
				const Vector2& move = mouseEvent->cursorPosition() - m_cursorPos;

				if (!move.fuzzyCompare({ 0, 0 })) {
					const float speed = 0.033f;

					//transform->setRotation(Quaternion::fromEulerDegrees(transform->rotation().eulerDegrees() + (Vector3(move.y, move.x, 0) * speed)));

					const auto& xRotation = Quaternion::angleAxisDegrees(move.x * speed, transform->up());
					const auto& yRotation = Quaternion::angleAxisDegrees(move.y * speed, transform->right());
					const auto& rotation = (xRotation * yRotation) * m_rotation;
					transform->setRotation(rotation);
				}
			}

			break;
		}

		case Event::MousePress:
			m_rotate = true;
			m_cursorPos = static_cast<MouseEvent*>(a_event.get())->cursorPosition();
			m_rotation = transform->rotation();
			break;

		case Event::MouseRelease:
			m_rotate = false;
			break;

		default:
			return;
		}

		a_event->setHandled();
	}
}
