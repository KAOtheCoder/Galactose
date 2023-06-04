#include "EditorLayer.h"
#include "EditorContext.h"

#include <Core/Events/KeyEvent.h>
#include <Core/Events/MouseEvent.h>
#include <Scene/Entity.h>
#include <Scene/Components/SpriteRenderer.h>
#include <Core/Application.h>
#include <Core/Window.h>

#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <nfd.hpp>

#include <filesystem>

using namespace Galactose;

namespace GalactoseEditor {
	EditorLayer::EditorLayer(Window* a_window) :
		m_sceneContext(std::make_shared<EditorContext>()),
		m_sceneViewport(m_sceneContext),
		m_gameViewport(m_sceneContext),
		m_sceneHierarchy(m_sceneContext),
		m_inspector(m_sceneContext)
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

		if (!std::filesystem::exists(io.IniFilename))
			m_layout = Layout::Default;

		const auto nfdResult = NFD_Init();
		GT_ASSERT(nfdResult == NFD_OKAY, "Failed to initialze NFD");

		m_menuBar.menus.push_back({ "File", {  
			{ "New Scene", { KeyEvent::KeyLeftControl, KeyEvent::KeyN }, [&]() { m_sceneContext->newScene(); } },
			{ "Open Scene", { KeyEvent::KeyLeftControl, KeyEvent::KeyO }, [&]() { m_sceneContext->openScene(); } },
			{ }, // Separator
			{ "Save", { KeyEvent::KeyLeftControl, KeyEvent::KeyS }, [&]() { m_sceneContext->save(); } },
			{ "Save As", { KeyEvent::KeyLeftControl, KeyEvent::KeyLeftShift, KeyEvent::KeyS }, [&]() { m_sceneContext->saveAs(); } },
			{ },
			{ "Exit", { KeyEvent::KeyLeftAlt, KeyEvent::KeyF4 }, [&]() { Application::instance()->exit(); } }
		} });

		m_menuBar.menus.push_back({ "Panels", {
			{ "Scene", { }, [&]() { m_sceneViewport.setVisible(true); } },
			{ "Game", { }, [&]() { m_gameViewport.setVisible(true); } },
			{ "Scene Hierarchy", { }, [&]() { m_sceneHierarchy.setVisible(true); } },
			{ "Inspector", { }, [&]() { m_inspector.setVisible(true); } }
		} });

		m_menuBar.menus.push_back({ "Layouts", {
			{ "Default", { }, [&]() { m_layout = Layout::Default; }}
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

		updateLayout();

		updateUpBar();
		
		if (m_sceneContext->isRunning()) {
			auto scene = m_sceneContext->scene();
			GT_ASSERT(scene, "Scene is null");
			scene->time().tick();
			scene->updateScripts();
		}

		m_sceneHierarchy.update();
		m_inspector.update();
		m_sceneViewport.update();
		m_gameViewport.update();

		ImGui::ShowDemoWindow();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Update and Render additional Platform Windows
		// (Platform functions may change the current OpenGL context
		GT_ASSERT(ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable, "ImGui viewports not enabled.");
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

	void EditorLayer::updateLayout() {
		auto dockSpaceId = ImGui::DockSpaceOverViewport();

		if (m_layout == Layout::Default) {
			ImGui::DockBuilderRemoveNode(dockSpaceId);
			ImGui::DockBuilderAddNode(dockSpaceId, ImGuiDockNodeFlags_DockSpace);

			const auto& workSize = ImGui::GetMainViewport()->WorkSize;
			ImGui::DockBuilderSetNodeSize(dockSpaceId, workSize);

			const float LEFT_RIGHT_SIZE_RATIO = 0.2f;
			const auto leftId = ImGui::DockBuilderSplitNode(dockSpaceId, ImGuiDir_Left, LEFT_RIGHT_SIZE_RATIO, nullptr, &dockSpaceId);
			const auto rightId = ImGui::DockBuilderSplitNode(dockSpaceId, ImGuiDir_Right, LEFT_RIGHT_SIZE_RATIO * (1 / (1 - LEFT_RIGHT_SIZE_RATIO)), nullptr, &dockSpaceId);
			
			ImGui::DockBuilderDockWindow(m_sceneHierarchy.name().c_str(), leftId);
			ImGui::DockBuilderDockWindow(m_inspector.name().c_str(), rightId);
			ImGui::DockBuilderDockWindow(m_sceneViewport.name().c_str(), dockSpaceId);
			ImGui::DockBuilderDockWindow(m_gameViewport.name().c_str(), dockSpaceId);
			ImGui::DockBuilderFinish(dockSpaceId);
		}

		m_layout = Layout::None;
	}

	void EditorLayer::updateUpBar() {
		// Inspired from ImGui::BeginMainMenuBar()
		const ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;
		const float height = m_menuBar.height() + m_toolBar.height();

		if (ImGui::BeginViewportSideBar("##UpBar", ImGui::GetMainViewport(), ImGuiDir_Up, height, windowFlags)) {
			m_menuBar.update();
			m_toolBar.update(m_sceneContext);
		}

		ImGui::End();
	}
}
