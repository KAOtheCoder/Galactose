#include "EditorLayer.h"
#include "EditorContext.h"

#include <Galactose/Core/Events/KeyEvent.h>
#include <Galactose/Core/Events/MouseEvent.h>
#include <Galactose/Core/Application.h>

#include <imgui_internal.h>

#include <filesystem>

using namespace Galactose;

namespace GalactoseEditor {
	EditorLayer::EditorLayer(Window* a_window, const std::filesystem::path& a_projectFilePath) :
		ImGuiLayer(a_window, true, true, "Editor.ini"),
		m_editorContext(std::make_shared<EditorContext>(a_projectFilePath)),
		m_sceneViewport(m_editorContext),
		m_gameViewport(m_editorContext),
		m_sceneHierarchy(m_editorContext),
		m_inspector(m_editorContext),
		m_assetExplorer(m_editorContext),
		m_layout(std::filesystem::exists(ImGui::GetIO().IniFilename) ? Layout::None : Layout::Default)
	{
		m_menuBar.menus.push_back({ "File", {  
			{ "New Scene", { KeyEvent::KeyLeftControl, KeyEvent::KeyN }, [&]() { m_editorContext->newScene(); } },
			{ "Open Scene", { KeyEvent::KeyLeftControl, KeyEvent::KeyO }, [&]() { m_editorContext->openScene(); } },
			{ }, // Separator
			{ "Save Scene", { KeyEvent::KeyLeftControl, KeyEvent::KeyS }, [&]() { m_editorContext->saveScene(); } },
			{ "Save Scene As", { KeyEvent::KeyLeftControl, KeyEvent::KeyLeftShift, KeyEvent::KeyS }, [&]() { m_editorContext->saveSceneAs(); } },
			{ },
			{ "Save Project", {}, [&]() { m_editorContext->saveProject(); } },
			{ },
			{ "Exit", { KeyEvent::KeyLeftAlt, KeyEvent::KeyF4 }, [&]() { Application::instance()->exit(); } }
		} });

		m_menuBar.menus.push_back({ "Panels", {
			{ "Scene", { }, [&]() { m_sceneViewport.setVisible(true); } },
			{ "Game", { }, [&]() { m_gameViewport.setVisible(true); } },
			{ "Scene Hierarchy", { }, [&]() { m_sceneHierarchy.setVisible(true); } },
			{ "Inspector", { }, [&]() { m_inspector.setVisible(true); } },
			{ "Asset Explorer", { }, [&]() { m_assetExplorer.setVisible(true); } }
		} });

		m_menuBar.menus.push_back({ "Layouts", {
			{ "Default", { }, [&]() { m_layout = Layout::Default; }}
		} });

		m_menuBar.menus.push_back({ "Build", {
			{ "Load Script(s)", { }, [&]() { m_editorContext->loadScripts(); }}
		} });
	}

	void EditorLayer::updateContent() {
		updateLayout();
		updateUpBar();
		
		if (m_editorContext->isRunning()) {
			auto scene = m_editorContext->scene();
			GT_ASSERT(scene, "Scene is null");
			scene->time().tick();
			scene->updateScripts();
		}

		m_sceneHierarchy.update();
		m_inspector.update();
		m_sceneViewport.update();
		m_gameViewport.update();
		m_assetExplorer.update();

		//ImGui::ShowDemoWindow();
	}

	void EditorLayer::onEvent(const std::shared_ptr<Event>& a_event) {
		m_menuBar.onEvent(a_event);

		auto panel = Panel::focusedPanel();
		if (panel) {
			if (a_event->type() == Event::MousePress && panel != Panel::hoveredPanel()) {
				a_event->setHandled();
				return;
			}

			panel->onEvent(a_event);
		}
	}

	void EditorLayer::updateLayout() {
		auto dockSpaceId = ImGui::DockSpaceOverViewport();

		if (m_layout == Layout::Default) {
			ImGui::DockBuilderRemoveNode(dockSpaceId);
			ImGui::DockBuilderAddNode(dockSpaceId, ImGuiDockNodeFlags_DockSpace);

			const auto& workSize = ImGui::GetMainViewport()->WorkSize;
			ImGui::DockBuilderSetNodeSize(dockSpaceId, workSize);

			const float DOCK_SIZE_RATIO = 0.2f;
			const auto rightId = ImGui::DockBuilderSplitNode(dockSpaceId, ImGuiDir_Right, DOCK_SIZE_RATIO, nullptr, &dockSpaceId);
			const auto bottomId = ImGui::DockBuilderSplitNode(dockSpaceId, ImGuiDir_Down, DOCK_SIZE_RATIO, nullptr, &dockSpaceId);
			const auto leftId = ImGui::DockBuilderSplitNode(dockSpaceId, ImGuiDir_Left, DOCK_SIZE_RATIO, nullptr, &dockSpaceId);

			ImGui::DockBuilderDockWindow(m_sceneHierarchy.name().c_str(), leftId);
			ImGui::DockBuilderDockWindow(m_inspector.name().c_str(), rightId);
			ImGui::DockBuilderDockWindow(m_sceneViewport.name().c_str(), dockSpaceId);
			ImGui::DockBuilderDockWindow(m_gameViewport.name().c_str(), dockSpaceId);
			ImGui::DockBuilderDockWindow(m_assetExplorer.name().c_str(), bottomId);
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
			m_toolBar.update(m_editorContext);
		}

		ImGui::End();
	}
}
