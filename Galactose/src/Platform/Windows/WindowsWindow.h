#pragma once
#include "Galactose/Core/Window.h"

#include <GLFW/glfw3.h>

#include "Galactose/Renderer/VertexArray.h"
#include "Galactose/Renderer/Shader.h"

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

		bool isVSync() const override { return m_vsync; }
		void setVSync(const bool a_vsync) override;

		void* nativeWindow() const override { return m_glfwWindow; }

	private:
		inline static std::shared_ptr<VertexArray> vao;
		inline static std::shared_ptr<Shader> shader;
		std::pair<int32_t, int32_t> size() const;

		inline static uint32_t s_glfwWindowCount = 0;

		GLFWwindow* m_glfwWindow = nullptr;
		std::string m_title;
		bool m_vsync;
	};
}