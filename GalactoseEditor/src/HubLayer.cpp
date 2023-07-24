#include "HubLayer.h"
#include "FileDialog.h"

#include <Galactose/Core/Window.h>
#include <Galactose/Core/Application.h>

#include <imgui.h>

#include <fstream>
#include <array>
#include <ctime>

using namespace Galactose;

namespace GalactoseEditor {
	HubLayer::HubLayer(Window* a_window) :
		ImGuiLayer(a_window, false, false, "Hub.ini"),
		m_window(a_window)
	{
		std::ifstream projectsFile(RECENT_PROJECTS_FILENAME);
		if (projectsFile.is_open()) {
			std::string line;

			while (std::getline(projectsFile, line)) {
				const auto delimIndex = line.find(';', 0);

				if (delimIndex == std::string::npos)
					continue;

				const std::filesystem::path& path = line.substr(0, delimIndex);
				
				if (std::filesystem::exists(path)) {
					int64_t time = 0;

					try {
						time = std::stoll(line.substr(delimIndex + 1));
					}
					catch (const std::exception& ex) {
						time = 0;
						std::cerr << ex.what() << std::endl;
					}

					m_projects.emplace(time, ProjectInfo(path, timeToString(time)));
				}
			}
		}
	}

	HubLayer::~HubLayer() {
		std::ofstream projectsFile(RECENT_PROJECTS_FILENAME);

		if (projectsFile.is_open()) {
			for (const auto& [time, project] : m_projects) {
				const auto& line = project.path.string() + ";" + std::to_string(time) + "\n";
				projectsFile.write(line.c_str(), line.size());
			}
		}
	}

	std::string HubLayer::timeToString(const int64_t a_ms) {
		const std::time_t seconds = a_ms / 1000;
		// TODO: consider using localtime_s
		return std::ctime(&seconds);
	}

	void HubLayer::insertProject() {
		const auto& duration = std::chrono::system_clock::now().time_since_epoch();
		const auto time = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
		m_projects.emplace(time, ProjectInfo(m_projectPath, timeToString(time)));
	}

	void HubLayer::updateContent() {
		ImGui::SetNextWindowPos({ 0, 0 });
		ImGui::SetNextWindowSize({ (float)m_window->width(), (float)m_window->height() });
		ImGui::Begin("Galactose Hub", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

		ImGui::SetWindowFontScale(2.f);
		ImGui::Text("Galactose");
		ImGui::SetWindowFontScale(1.f);

		ImGui::Dummy({ 0, ImGui::GetTextLineHeightWithSpacing() });

		if (ImGui::BeginTable("Project", 3)) {
			ImGui::TableSetupColumn(nullptr, ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn(nullptr, ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableSetupColumn(nullptr, ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableNextRow();

			ImGui::TableSetColumnIndex(0);
			ImGui::Text("Projects");

			ImGui::TableSetColumnIndex(1);
			if (ImGui::Button("Open")) {
				m_projectPath = FileDialog::open({ { "Project" , "pro" } });
				insertProject();
			}

			ImGui::TableSetColumnIndex(2);
			if (ImGui::Button("Create")) {
				m_projectPath = FileDialog::save({ { "Project" , ""}});
				std::filesystem::create_directory(m_projectPath);
				m_projectPath = m_projectPath / (m_projectPath.stem().string() + ".pro");
				std::ofstream projectFile(m_projectPath);
				insertProject();
			}
			
			ImGui::EndTable();
		}

		ImGui::Separator();

		for (const auto& [time, project] : m_projects) {
			const auto& text = project.path.string() + "\n" + project.time;
			const bool selected = ImGui::Selectable(("##" + project.path.string()).c_str(), false, 0, { 0, 2.25f * ImGui::GetTextLineHeightWithSpacing() });
			ImGui::SameLine();
			if (ImGui::BeginTable(project.path.string().c_str(), 2)) {
				ImGui::TableSetupColumn(nullptr, ImGuiTableColumnFlags_WidthStretch);
				ImGui::TableSetupColumn(nullptr, ImGuiTableColumnFlags_WidthFixed);
				
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::SetWindowFontScale(1.25f);
				ImGui::Text(project.path.stem().string().c_str());
				ImGui::SetWindowFontScale(1.f);

				ImGui::TableSetColumnIndex(1);
				ImGui::Text(project.time.c_str());

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text(project.path.string().c_str());

				ImGui::EndTable();
			}

			//ImGui::Selectable((project.path.string() + "\n" + project.time).c_str())
			if (selected) {
				// update time
				m_projectPath = project.path;
				m_projects.erase(time);
				insertProject();

				break;
			}
		}

		ImGui::End();

		if (!m_projectPath.empty())
			Application::instance()->exit();

		//ImGui::ShowDemoWindow();
	}
}
