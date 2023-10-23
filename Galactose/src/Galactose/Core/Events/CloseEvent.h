#pragma once

#include "Event.h"
#include "Galactose/Core/Window.h"

namespace Galactose {
	class CloseEvent : public Event {
	public:
		GT_EVENT_TYPE_IMP(Close)

		CloseEvent(const std::shared_ptr<Window>& a_window) : m_window(a_window) {}

		std::shared_ptr<Window> window() const { return m_window.lock(); }

		void process() override { 
			const auto& sharedWindow = window();
			if (sharedWindow) {
				sharedWindow->onEvent(shared_from_this());

				if (!isHandled())
					sharedWindow->close();
			}
		}

	private:
		const std::weak_ptr<Window> m_window;
	};
}
