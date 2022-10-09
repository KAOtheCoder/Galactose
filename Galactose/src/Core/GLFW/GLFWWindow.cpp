#include "GalactosePCH.h"
#include "GLFWWindow.h"

#include "Renderer/Renderer.h"
#include "Core/Events/KeyPressEvent.h"

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

		glfwSetKeyCallback(m_glfwWindow, [](GLFWwindow* window, int key, int scancode, int action, int modes) {
			if (action == GLFW_PRESS)
				Application::instance()->postEvent(std::make_shared<KeyPressEvent>(static_cast<KeyEvent::Key>(key)));
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

			Renderer::renderer()->clear();
			Renderer::renderer()->drawSprite(Vector3(), { 1.0f, 1.0f });

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
