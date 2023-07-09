#include "HubLayer.h"
#include "FileDialog.h"

#include <Galactose/Core/Window.h>
#include <Galactose/Core/Application.h>

#include <imgui.h>

using namespace Galactose;

namespace GalactoseEditor {
	HubLayer::HubLayer(Window* a_window) :
		ImGuiLayer(a_window, false, false, "Hub.ini"),
		m_window(a_window)
	{}

	void HubLayer::updateContent() {
		ImGui::SetNextWindowPos({ 0, 0 });
		ImGui::SetNextWindowSize({ (float)m_window->width(), (float)m_window->height() });
		ImGui::Begin("Galactose Hub", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

		if (ImGui::BeginTable("Project", 3)) {
			ImGui::TableSetupColumn(nullptr, ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn(nullptr, ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableSetupColumn(nullptr, ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableNextRow();

			ImGui::TableSetColumnIndex(0);
			ImGui::Text("Projects");

			ImGui::TableSetColumnIndex(1);
			if (ImGui::Button("Open"))
				m_projectPath = FileDialog::open({ { "Project" , "pro" } });

			ImGui::TableSetColumnIndex(2);
			if (ImGui::Button("Create"))
				m_projectPath = FileDialog::save({ { "Project" , "pro" } });
			
			ImGui::EndTable();
		}

		ImGui::Separator();

		// TODO: list recent projects

		ImGui::End();

		if (!m_projectPath.empty())
			Application::instance()->exit();
	}
}
