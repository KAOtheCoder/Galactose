#pragma once

#include "Event.h"

namespace Galactose {
	class Window;

	class InputEvent : public Event {
	public:
		GT_EVENT_TYPE_IMP(Input)

		std::shared_ptr<Window> window() const { return m_window.lock(); }

		void process() override;

	protected:
		InputEvent(const std::shared_ptr<Window>& a_window) : m_window(a_window) {}

	private:
		const std::weak_ptr<Window> m_window;
	};
}
