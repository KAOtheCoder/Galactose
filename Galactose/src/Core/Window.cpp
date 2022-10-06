#include "GalactosePCH.h"
#include "Window.h"
#include "Core/GLFW/GLFWWindow.h"
#include "Core/Application.h"

namespace Galactose {
	std::shared_ptr<Window> Window::create(const std::string& a_title, const int32_t a_width, const int32_t a_height) {
		const auto& window = std::make_shared<GLFWWindow>(a_title, a_width, a_height);
		s_windows.push_back(window);

		return window;
	}

	Window::~Window() {
		for (int i = 0; i < s_windows.size(); ++i)
			if (s_windows[i].get() == this)
				s_windows.erase(s_windows.begin() + i);
	}

	bool Window::areAllWindowsClosed() {
		for (auto window : s_windows)
			if (!window->isClosed())
				return false;

		return true;
	}
}
