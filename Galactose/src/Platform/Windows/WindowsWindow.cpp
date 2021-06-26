#include "WindowsWindow.h"

namespace Galactose {
	WindowsWindow::WindowsWindow(const std::string& a_title, const int32_t a_width, const int32_t a_height)
		: m_title(a_title)
	{
		if (s_windows.empty()) {
			const int success = glfwInit();
			//ASSERT(success == GLFW_FALSE)
		}

		m_glfwWindow = glfwCreateWindow(a_width, a_height, a_title.c_str(), nullptr, nullptr);
		//ASSERT(!m_GLFWWindow.get())

		glfwSetWindowUserPointer(m_glfwWindow, this);

		glfwSetWindowCloseCallback(m_glfwWindow, [](GLFWwindow* window) {
			static_cast<WindowsWindow*>(glfwGetWindowUserPointer(window))->close();
		});
	}

	WindowsWindow::~WindowsWindow() {
		close();
	}

	std::pair<int32_t, int32_t> WindowsWindow::size() const {
		std::pair<int32_t, int32_t> pair(0, 0);
		if (m_glfwWindow)
			glfwGetWindowSize(m_glfwWindow, &pair.first, &pair.second);

		return pair;
	}

	void WindowsWindow::update() {
		glfwPollEvents();
	}

	void WindowsWindow::close() {
		if (m_glfwWindow) {
			glfwDestroyWindow(m_glfwWindow);
			m_glfwWindow = nullptr;

			if (s_windows.size() == 1)
				glfwTerminate();
		}
	}
}
