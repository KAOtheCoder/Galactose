#pragma once

#include "Event.h"
#include "Core/Window.h"

namespace Galactose {
	class InputEvent : public Event {
	public:
		GT_EVENT_TYPE_IMP(Input)

		std::shared_ptr<Window> window() const { return m_window; }

		void process() override { m_window->onEvent(shared_from_this()); }

	protected:
		InputEvent(const std::shared_ptr<Window>& a_window) : m_window(a_window) {}

	private:
		const std::shared_ptr<Window> m_window;
	};
}
