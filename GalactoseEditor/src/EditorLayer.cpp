#include "EditorLayer.h"
#include "EditorContext.h"
#include "SideBars/MenuBar.h"
#include "SideBars/ToolBar.h"
#include "SideBars/StatusBar.h"

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
		m_upBar("##UpBar", ImGui::GetMainViewport(), ImGuiDir_::ImGuiDir_Up),
		m_downBar("##DownBar", ImGui::GetMainViewport(), ImGuiDir_::ImGuiDir_Down),
		m_layout(std::filesystem::exists(ImGui::GetIO().IniFilename) ? Layout::None : Layout::Default)
	{
		const auto& menuBar = std::make_shared<MenuBar>();

		menuBar->menus.push_back({ "File", {  
			{ "New Scene", KeyEvent::Control, KeyEvent::KeyN, [&]() { m_editorContext->newScene(); } },
			{ "Open Scene", KeyEvent::Control, KeyEvent::KeyO, [&]() { m_editorContext->openScene(); } },
			{ }, // Separator
			{ "Save Scene", KeyEvent::Control, KeyEvent::KeyS, [&]() { m_editorContext->saveScene(); } },
			{ "Save Scene As", KeyEvent::Control | KeyEvent::Shift, KeyEvent::KeyS, [&]() { m_editorContext->saveSceneAs(); } },
			{ },
			{ "Save Project", {}, [&]() { m_editorContext->saveProject(); }},
			{ },
			{ "Exit", KeyEvent::Control, KeyEvent::KeyE, [&]() { m_openExitDialog = true; }}
		} });

		menuBar->menus.push_back({ "Panels", {
			{ "Scene", { }, [&]() { m_sceneViewport.setVisible(true); } },
			{ "Game", { }, [&]() { m_gameViewport.setVisible(true); } },
			{ "Scene Hierarchy", { }, [&]() { m_sceneHierarchy.setVisible(true); } },
			{ "Inspector", { }, [&]() { m_inspector.setVisible(true); } },
			{ "Asset Explorer", { }, [&]() { m_assetExplorer.setVisible(true); } }
		} });

		menuBar->menus.push_back({ "Layouts", {
			{ "Default", { }, [&]() { m_layout = Layout::Default; }}
		} });

		menuBar->menus.push_back({ "Build", {
			{ "Load Script(s)", { }, [&]() { m_editorContext->loadScripts(); }}
		} });

		m_upBar.addSideBar(menuBar);
		m_upBar.addSideBar(std::make_shared<ToolBar>(m_editorContext));
		m_downBar.addSideBar(std::make_shared<StatusBar>(m_editorContext));
	}

	void EditorLayer::updateContent() {
		updateLayout();
		m_upBar.update();
		m_downBar.update();
		
		if (m_editorContext->state() == EditorContext::Playing) {
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

		if (m_openExitDialog) {
			ImGui::OpenPopup("Exit Galactose");

			if (ImGui::BeginPopupModal("Exit Galactose", &m_openExitDialog, ImGuiWindowFlags_AlwaysAutoResize)) {
				ImGui::Text("Do you really want to quit?");

				if (ImGui::BeginTable("Buttons", 3)) {
					ImGui::TableSetupColumn(nullptr, ImGuiTableColumnFlags_WidthStretch);
					ImGui::TableSetupColumn(nullptr, ImGuiTableColumnFlags_WidthFixed);
					ImGui::TableSetupColumn(nullptr, ImGuiTableColumnFlags_WidthFixed);

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(1);
					if (ImGui::Button("Yes"))
						Application::instance()->exit();

					ImGui::TableSetColumnIndex(2);
					if (ImGui::Button("No")) {
						ImGui::CloseCurrentPopup();
						m_openExitDialog = false;
					}

					ImGui::EndTable();
				}

				ImGui::EndPopup();
			}
		}

		//ImGui::ShowDemoWindow();
	}

	void EditorLayer::onEvent(const std::shared_ptr<Event>& a_event) {
		if (a_event->type() == Event::Close) {
			m_openExitDialog = true;
			a_event->setHandled();
			return;
		}

		m_upBar.onEvent(a_event);
		m_downBar.onEvent(a_event);

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
}
