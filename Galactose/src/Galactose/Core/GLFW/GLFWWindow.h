#pragma once
#include "Galactose/Core/Window.h"

struct GLFWwindow;

namespace Galactose {
	class GLFWWindow : public Window
	{
	public:
		~GLFWWindow() override;

		std::string title() const override { return m_title; }
		int32_t width() const override { return size().first; }
		int32_t height() const override { return size().second; }
		
		bool isClosed() const override { return !m_glfwWindow; }

		void update() override;
		void close() override;

		bool vsync() const override { return m_vsync; }
		void setVSync(const bool a_vsync) override;

		bool isMouseButtonPressed(const MouseEvent::Button button) const override;

		void* nativeWindow() const override { return m_glfwWindow; }

	private:
		static std::shared_ptr<Window> toWindow(GLFWwindow* window);
		static GLFWWindow* getCurrentContext();
		static void setCurrentContext(GLFWWindow* context);

		static void errorCallBack(const int error, const char* description);
		static void windowCloseCallback(GLFWwindow* a_window);
		static void keyCallback(GLFWwindow* window, const int key, const int scancode, const int action, const int modes);
		static void cursorPosCallback(GLFWwindow* window, const double x, const double y);
		static void mouseButtonCallback(GLFWwindow* window, const int button, const int action, const int mods);

		GLFWWindow(const std::string& title, const int32_t width, const int32_t height);

		std::pair<int32_t, int32_t> size() const;

		inline static uint32_t s_glfwWindowCount = 0;

		GLFWwindow* m_glfwWindow = nullptr;
		std::string m_title;
		bool m_vsync;

		friend class Window; // Window calls constructor in create
	};
}