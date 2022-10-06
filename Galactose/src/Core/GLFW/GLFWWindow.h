#pragma once
#include "Core/Window.h"

#include <GLFW/glfw3.h>

namespace Galactose {
	class GLFWWindow : public Window
	{
	public:
		GLFWWindow(const std::string& a_title, const int32_t a_width, const int32_t a_height);
		~GLFWWindow() override;

		std::string title() const override { return m_title; }
		int32_t width() const override { return size().first; }
		int32_t height() const override { return size().second; }
		
		bool isClosed() const override { return !m_glfwWindow; }

		void update() override;
		void close() override;

		bool vsync() const override { return m_vsync; }
		void setVSync(const bool a_vsync) override;

		void* nativeWindow() const override { return m_glfwWindow; }

	private:
		std::pair<int32_t, int32_t> size() const;

		inline static uint32_t s_glfwWindowCount = 0;

		GLFWwindow* m_glfwWindow = nullptr;
		std::string m_title;
		bool m_vsync;
	};
}