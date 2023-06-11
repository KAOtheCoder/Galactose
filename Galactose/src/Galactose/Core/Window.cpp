#include "Window.h"
#include "Galactose/Core/GLFW/GLFWWindow.h"
#include "Galactose/Core/Application.h"
#include "Galactose/Core/Events/Event.h"

namespace Galactose {
	Window::Window() {
		s_windows.insert(this);
	}

	Window::~Window() {
		s_windows.erase(this);
	}

	std::shared_ptr<Window> Window::create(const std::string& a_title, const int32_t a_width, const int32_t a_height) {
		// std::make_shared can't access private constructor
		return std::shared_ptr<GLFWWindow>(new GLFWWindow(a_title, a_width, a_height));
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

	Window* Window::getCurrentContext() { return GLFWWindow::getCurrentContext(); }
	void Window::setCurrentContext(Window* a_context) { GLFWWindow::setCurrentContext(static_cast<GLFWWindow*>(a_context)); }
}
