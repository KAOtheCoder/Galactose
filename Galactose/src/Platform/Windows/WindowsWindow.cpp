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

	int32_t WindowsWindow::width() const {
		int width, height;
		glfwGetWindowSize(m_glfwWindow, &width, &height);
		return width;
	}

	int32_t WindowsWindow::height() const {
		int width, height;
		glfwGetWindowSize(m_glfwWindow, &width, &height);
		return height;
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
