#include "GLFWWindow.h"

#include "Core/Application.h"
#include "Core/Events/KeyEvent.h"
#include "Core/Events/MouseEvent.h"
#include "Core/Events/CloseEvent.h"

#include <GLFW/glfw3.h>

namespace Galactose {
	GLFWWindow* GLFWWindow::getCurrentContext() { return static_cast<GLFWWindow*>(toWindow(glfwGetCurrentContext()).get()); }
	void GLFWWindow::setCurrentContext(GLFWWindow* a_context) { glfwMakeContextCurrent(a_context->m_glfwWindow); }

	GLFWWindow::GLFWWindow(const std::string& a_title, const int32_t a_width, const int32_t a_height)
		: m_title(a_title),
		  m_vsync(true)
	{
		if (s_glfwWindowCount == 0) {
			const int success = glfwInit();
			GT_ASSERT(success == GLFW_TRUE, "GLFW initialization failed.");
			glfwSetErrorCallback(errorCallBack);
		}

		m_glfwWindow = glfwCreateWindow(a_width, a_height, a_title.c_str(), nullptr, nullptr);
		GT_ASSERT(m_glfwWindow, "GLFW window creation failed.");
		++s_glfwWindowCount;

		glfwSetWindowUserPointer(m_glfwWindow, this);

		glfwSetWindowCloseCallback(m_glfwWindow, windowCloseCallback);
		glfwSetKeyCallback(m_glfwWindow, keyCallback);
		glfwSetCursorPosCallback(m_glfwWindow, cursorPosCallback);
		glfwSetMouseButtonCallback(m_glfwWindow, mouseButtonCallback);

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
			for (const auto& layer : layers)
				layer->onUpdate();

			glfwMakeContextCurrent(m_glfwWindow); // current context might be changed by ImGui
			glfwSwapBuffers(m_glfwWindow);
			glfwPollEvents();
		}
	}

	void GLFWWindow::close() {
		layers.clear();

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

	std::shared_ptr<Window> GLFWWindow::toWindow(GLFWwindow* a_window) {
		return static_cast<GLFWWindow*>(glfwGetWindowUserPointer(a_window))->shared_from_this();
	}

	void GLFWWindow::errorCallBack(const int a_error, const char* a_description) {
		std::cerr << "GLFW Error " << a_error << ": " << a_description << std::endl;
	}

	void GLFWWindow::windowCloseCallback(GLFWwindow* a_window) {
		Application::instance()->postEvent(std::make_shared<CloseEvent>(toWindow(a_window)));
	}

	void GLFWWindow::keyCallback(GLFWwindow* a_window, const int a_key, const int a_scancode, const int a_action, const int a_modes) {
		std::shared_ptr<Event> event;
		const auto& window = toWindow(a_window);
		const auto key = static_cast<KeyEvent::Key>(a_key);

		switch (a_action) {
		case GLFW_PRESS: event = std::make_shared<KeyPressEvent>(window, key);
			break;
		case GLFW_REPEAT: event = std::make_shared<KeyPressEvent>(window, key, true);
			break;
		case GLFW_RELEASE: event = std::make_shared<KeyReleaseEvent>(window, key);
			break;
		default:
			GT_ASSERT(false, "Unknown key event.");
		}

		Application::instance()->postEvent(event);
	}

	void GLFWWindow::cursorPosCallback(GLFWwindow* a_window, const double a_x, const double a_y) {
		Application::instance()->postEvent(std::make_shared<MouseMoveEvent>(toWindow(a_window), Vector2(a_x, a_y)));
	}

	void GLFWWindow::mouseButtonCallback(GLFWwindow* a_window, const int a_button, const int a_action, const int a_mods) {
		std::shared_ptr<Event> event;
		const auto& window = toWindow(a_window);
		const auto button = static_cast<MouseEvent::Button>(a_button);
		double x, y;
		glfwGetCursorPos(a_window, &x, &y);
		const Vector2 pos(x, y);

		switch (a_action) {
		case GLFW_PRESS: event = std::make_shared<MousePressEvent>(window, pos, button);
			break;
		case GLFW_RELEASE: event = std::make_shared<MouseReleaseEvent>(window, pos, button);
			break;
		default:
			GT_ASSERT(false, "Unknown mouse event.");
		}

		Application::instance()->postEvent(event);
	}
}
