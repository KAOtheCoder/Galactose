#include "GalactosePCH.h"
#include "WindowsWindow.h"

#include "Galactose/Renderer/Renderer.h"

namespace Galactose {
	WindowsWindow::WindowsWindow(const std::string& a_title, const int32_t a_width, const int32_t a_height)
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
			static_cast<WindowsWindow*>(glfwGetWindowUserPointer(window))->close();
		});

		setVSync(true);
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
		if (m_glfwWindow) {
			glfwSwapBuffers(m_glfwWindow);

			if (!vao) {
				vao = VertexArray::create();

				float vertices[] = {
				-0.5f, -0.5f, 0.0f,
				0.5f, -0.5f, 0.0f,
				0.0f, 0.5f, 0.0f
				};

				uint32_t indices[] = {
					2, 1, 0
				};

				vao.get()->addVertexBuffer(VertexBuffer::create(vertices, 9 * sizeof(float)));
				vao.get()->setIndexBuffer(IndexBuffer::create(indices, 3));
			}

			Renderer::renderer()->clear();
			Renderer::renderer()->drawVertexArrayIndexed(vao);

			glfwPollEvents();
		}
	}

	void WindowsWindow::close() {
		if (m_glfwWindow) {
			glfwDestroyWindow(m_glfwWindow);
			m_glfwWindow = nullptr;

			if (--s_glfwWindowCount == 0)
				glfwTerminate();
		}
	}

	void WindowsWindow::setVSync(const bool a_vsync) {
		glfwSwapInterval(a_vsync ? 1 : 0);
		m_vsync = a_vsync;
	}
}
