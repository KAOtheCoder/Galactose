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
		int32_t width() const override { return size().first; }
		int32_t height() const override { return size().second; }
		
		bool isClosed() const override { return !m_glfwWindow; }

		void update() override;
		void close() override;

	private:
		std::pair<int32_t, int32_t> size() const;

		GLFWwindow* m_glfwWindow = nullptr;
		std::string m_title;
	};
}