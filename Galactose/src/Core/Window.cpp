#include "Window.h"
#include "Core/GLFW/GLFWWindow.h"
#include "Core/Application.h"
#include "Core/Events/Event.h"

namespace Galactose {
	std::shared_ptr<Window> Window::create(const std::string& a_title, const int32_t a_width, const int32_t a_height) {
		std::shared_ptr<GLFWWindow> window(new GLFWWindow(a_title, a_width, a_height));
		s_windows.push_back(window);

		return window;
	}

	bool Window::areAllWindowsClosed() {
		for (const auto& window : s_windows)
			if (!window->isClosed())
				return false;

		return true;
	}

	void Window::onEvent(const std::shared_ptr<Event>& a_event) {
		for (auto& layer : layers) {
			if (a_event->isHandled())
				return;

			layer->onEvent(a_event);
		}
	}
}
