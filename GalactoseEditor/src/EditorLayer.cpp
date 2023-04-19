#include "EditorLayer.h"
#include "EditorSceneData.h"

#include <Core/Events/KeyEvent.h>
#include <Core/Events/MouseEvent.h>
#include <Scene/Entity.h>
#include <Scene/Components/SpriteRenderer.h>
#include <Core/Application.h>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <nfd.hpp>

using namespace Galactose;

namespace GalactoseEditor {
	EditorLayer::EditorLayer(Window* a_window) :
		m_sceneData(std::make_shared<EditorSceneData>()),
		m_sceneViewport(m_sceneData),
		m_gameViewport(m_sceneData),
		m_sceneHierarchy(m_sceneData),
		m_inspector(m_sceneData)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		auto& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;
		//io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;

		ImGui::StyleColorsDark();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(a_window->nativeWindow()), true);
		ImGui_ImplOpenGL3_Init("#version 410");

		const auto nfdResult = NFD_Init();
		GT_ASSERT(nfdResult == NFD_OKAY, "Failed to initialze NFD");

		m_menuBar.menus.push_back({ "File", {  
			{ "New Scene", { KeyEvent::KeyLeftControl, KeyEvent::KeyN }, [&]() { m_sceneData->newScene(); } },
			{ "Open Scene", { KeyEvent::KeyLeftControl, KeyEvent::KeyO }, [&]() { m_sceneData->openScene(); } },
			{ }, // Separator
			{ "Save", { KeyEvent::KeyLeftControl, KeyEvent::KeyS }, [&]() { m_sceneData->save(); } },
			{ "Save As", { KeyEvent::KeyLeftControl, KeyEvent::KeyLeftShift, KeyEvent::KeyS }, [&]() { m_sceneData->saveAs(); } },
			{ },
			{ "Exit", { KeyEvent::KeyLeftAlt, KeyEvent::KeyF4 }, [&]() { Application::instance()->exit(); } }
		} });

		m_menuBar.menus.push_back({ "Panels", {
			{ "Scene", { }, [&]() { m_sceneViewport.setVisible(true); } },
			{ "Scene Hierarchy", { }, [&]() { m_sceneHierarchy.setVisible(true); } },
			{ "Inspector", { }, [&]() { m_inspector.setVisible(true); } }
		} });
	}

	EditorLayer::~EditorLayer() {
		NFD_Quit();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void EditorLayer::onUpdate() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		m_menuBar.draw();

		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

		m_sceneHierarchy.update();
		m_inspector.update();
		m_sceneViewport.update();
		m_gameViewport.update();

		ImGui::ShowDemoWindow();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Update and Render additional Platform Windows
		// (Platform functions may change the current OpenGL context
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			auto context = Window::getCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			Window::setCurrentContext(context);
		}
	}

	void EditorLayer::onEvent(const std::shared_ptr<Event>& a_event) {
		m_menuBar.onEvent(a_event);

		auto panel = Panel::focusedPanel();
		if (panel)
			panel->onEvent(a_event);
	}
}
