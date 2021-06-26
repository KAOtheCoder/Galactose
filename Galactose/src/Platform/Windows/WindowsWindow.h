#pragma once
#include "Galactose/Core/Window.h"

#include <GLFW/glfw3.h>

namespace Galactose {
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const std::string& a_title, const int32_t a_width, const int32_t a_height);
		~WindowsWindow() override;

		std::string title() const override { return m_title; }
		int32_t width() const override;
		int32_t height() const override;
		
		bool isClosed() const override { return !m_glfwWindow; }

		void update() override;
		void close() override;

	private:
		GLFWwindow* m_glfwWindow = nullptr;
		std::string m_title;
	};
}