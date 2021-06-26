#include "Window.h"
#include "Platform/Windows/WindowsWindow.h"
#include "Galactose/Core/Application.h"

namespace Galactose {
	std::shared_ptr<Window> Window::create(const std::string& a_title, const int32_t a_width, const int32_t a_height) {
		auto app = Application::instance();
		//ASSERT(!app)
		auto& window = std::make_shared<WindowsWindow>(a_title, a_width, a_height);
		s_windows.emplace_back(window);

		return window;
	}

	Window::~Window() {
		for (int i = 0; i < s_windows.size(); ++i)
			if (s_windows[i].get() == this)
				s_windows.erase(s_windows.begin() + i);
	}

	bool Window::areAllWindowsClosed() {
		for (auto window : s_windows)
			if (!window.get()->isClosed())
				return false;

		return true;
	}
}
