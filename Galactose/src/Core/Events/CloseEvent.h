#pragma once

#include "Event.h"
#include "Core/Window.h"

namespace Galactose {
	class CloseEvent : public Event {
	public:
		GT_EVENT_TYPE_IMP(Close)

		CloseEvent(const std::shared_ptr<Window>& a_window) : m_window(a_window) {}

		std::shared_ptr<Window> window() const { return m_window; }

		void process() override { m_window->close(); }

	private:
		const std::shared_ptr<Window> m_window;
	};
}
