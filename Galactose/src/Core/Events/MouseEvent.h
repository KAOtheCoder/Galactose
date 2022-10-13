#pragma once

#include "Event.h"
#include "Core/Math.h"

namespace Galactose {
	class MouseEvent : public InputEvent {
	public:
		enum Button {
			Left,
			Right,
			Middle,
			Button4,
			Button5,
			Button6,
			Button7,
			Button8
		};

		GT_EVENT_TYPE_IMP(Mouse)

		std::string toString() const override {
			return name() + ": " + m_cursorPos.toString();
		}

	protected:
		MouseEvent(const std::shared_ptr<Window>& a_window, const Vector2& a_pos) : InputEvent(a_window), m_cursorPos(a_pos) {}

	private:
		const Vector2 m_cursorPos;
	};

	class MouseMoveEvent : public MouseEvent {
	public:
		GT_EVENT_TYPE_IMP(MouseMove)

		MouseMoveEvent(const std::shared_ptr<Window>& a_window, const Vector2& a_pos) : MouseEvent(a_window, a_pos) {}
	};

	class MouseButtonEvent : public MouseEvent {
	public:
		std::string toString() const override {
			return MouseEvent::toString() + " " + std::to_string(m_button);
		}

	protected:
		MouseButtonEvent(const std::shared_ptr<Window>& a_window, const Vector2& a_pos, const Button a_button) : MouseEvent(a_window, a_pos), m_button(a_button) {}

	private:
		const Button m_button;
	};

	class MousePressEvent : public MouseButtonEvent {
	public:
		GT_EVENT_TYPE_IMP(MousePress)

		MousePressEvent(const std::shared_ptr<Window>& a_window, const Vector2& a_pos, const Button a_button) : MouseButtonEvent(a_window, a_pos, a_button) {}
	};

	class MouseReleaseEvent : public MouseButtonEvent {
	public:
		GT_EVENT_TYPE_IMP(MouseRelease)

		MouseReleaseEvent(const std::shared_ptr<Window>& a_window, const Vector2& a_pos, const Button a_button) : MouseButtonEvent(a_window, a_pos, a_button) {}
	};
}
