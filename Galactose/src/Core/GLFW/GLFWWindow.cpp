#include "GalactosePCH.h"
#include "GLFWWindow.h"

#include "Core/Application.h"
#include "Core/Events/KeyEvent.h"
#include "Core/Events/MouseEvent.h"

namespace Galactose {
	GLFWWindow::GLFWWindow(const std::string& a_title, const int32_t a_width, const int32_t a_height)
		: m_title(a_title),
		  m_vsync(true)
	{
		if (s_glfwWindowCount == 0) {
			const int success = glfwInit();
			GT_ASSERT(success == GLFW_TRUE, "GLFW initialization failed.");
		}

		m_glfwWindow = glfwCreateWindow(a_width, a_height, a_title.c_str(), nullptr, nullptr);
		GT_ASSERT(m_glfwWindow, "GLFW window creation failed.");
		++s_glfwWindowCount;

		glfwSetWindowUserPointer(m_glfwWindow, this);

		glfwSetWindowCloseCallback(m_glfwWindow, [](GLFWwindow* window) {
			static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window))->close();
		});

		glfwSetKeyCallback(m_glfwWindow, [](GLFWwindow* a_window, int a_key, int a_scancode, int a_action, int a_modes) {
			std::shared_ptr<Event> event;
			const auto key = static_cast<KeyEvent::Key>(a_key);

			switch (a_action) {
			case GLFW_PRESS: event = std::make_shared<KeyPressEvent>(key);
				break;
			case GLFW_REPEAT: event = std::make_shared<KeyRepeatEvent>(key);
				break;
			case GLFW_RELEASE: event = std::make_shared<KeyReleaseEvent>(key);
				break;
			default:
				GT_ASSERT(false, "Unknown key event.");
			}

			Application::instance()->postEvent(event);
		});

		glfwSetCursorPosCallback(m_glfwWindow, [](GLFWwindow* a_window, double a_x, double a_y) {
			Application::instance()->postEvent(std::make_shared<MouseMoveEvent>(Vector2(a_x, a_y)));
		});

		glfwSetMouseButtonCallback(m_glfwWindow, [](GLFWwindow* a_window, int a_button, int a_action, int a_mods) {
			std::shared_ptr<Event> event;
			const auto button = static_cast<MouseEvent::Button>(a_button);
			double x, y;
			glfwGetCursorPos(a_window, &x, &y);
			const Vector2 pos(x, y);

			switch (a_action) {
			case GLFW_PRESS: event = std::make_shared<MousePressEvent>(pos, button);
				break;
			case GLFW_RELEASE: event = std::make_shared<MouseReleaseEvent>(pos, button);
				break;
			default:
				GT_ASSERT(false, "Unknown mouse event.");
			}

			Application::instance()->postEvent(event);
		});

		setVSync(true);
	}

	GLFWWindow::~GLFWWindow() {
		close();
	}

	std::pair<int32_t, int32_t> GLFWWindow::size() const {
		std::pair<int32_t, int32_t> pair(0, 0);
		if (m_glfwWindow)
			glfwGetWindowSize(m_glfwWindow, &pair.first, &pair.second);

		return pair;
	}

	void GLFWWindow::update() {
		if (m_glfwWindow) {
			glfwSwapBuffers(m_glfwWindow);

			for (const auto& layer : layers)
				layer->onUpdate();

			glfwPollEvents();
		}
	}

	void GLFWWindow::close() {
		if (m_glfwWindow) {
			glfwDestroyWindow(m_glfwWindow);
			m_glfwWindow = nullptr;

			if (--s_glfwWindowCount == 0)
				glfwTerminate();
		}
	}

	void GLFWWindow::setVSync(const bool a_vsync) {
		glfwSwapInterval(a_vsync ? 1 : 0);
		m_vsync = a_vsync;
	}
}
