#include "AssetExplorer.h"
#include "EditorContext.h"

#include <Galactose/Renderer/Texture.h>

using namespace Galactose;

namespace GalactoseEditor {
	AssetExplorer::AssetExplorer(const std::shared_ptr<EditorContext>& a_editorContext) : 
		Panel("Asset Explorer"),
		m_editorContext(a_editorContext),
		m_directoryPath(a_editorContext->project().directory())
	{
		m_icons.emplace("Up", Texture::create("assets/textures/Up.png"));
		m_icons.emplace("Folder", Texture::create("assets/textures/Folder256.png"));
		m_icons.emplace("File", Texture::create("assets/textures/File256.png"));
		m_icons.emplace("NotIncluded", Texture::create("assets/textures/NotIncluded.png"));
	}

	void AssetExplorer::onUpdate() {
		const auto& style = ImGui::GetStyle();
		// always ignore vertical scrollbar area to avoid flickering 
		const auto availableWidth = ImGui::GetContentRegionAvail().x - (ImGui::GetScrollMaxY() > 0 ? 0 : style.ScrollbarSize);
		const ImVec2 tableSize(availableWidth, 0);
		const float fontSize = ImGui::GetFontSize();
		const float minThumnailSize = 2 * fontSize;
		auto& project = m_editorContext->project();
		const auto& projectDirectory = project.directory();
		const bool inTopPath = m_directoryPath == projectDirectory;

		if (ImGui::BeginTable("##Controls", 3, ImGuiTableFlags_NoPadOuterX, tableSize)) {
			const float buttonSize = fontSize + 2 * style.FramePadding.x;

			ImGui::TableSetupColumn(nullptr, ImGuiTableColumnFlags_WidthFixed, buttonSize);
			ImGui::TableSetupColumn(nullptr, ImGuiTableColumnFlags_WidthStretch, 1);
			ImGui::TableSetupColumn(nullptr, ImGuiTableColumnFlags_WidthFixed, 6 * buttonSize);

			ImGui::TableNextRow();

			if (!inTopPath) {
				ImGui::TableSetColumnIndex(0);

				if (ImGui::ImageButton("Up", (void*)(intptr_t)m_icons["Up"]->rendererId(), { fontSize, fontSize }, { 0, 1 }, { 1, 0 }))
					m_directoryPath = m_directoryPath.parent_path();

				ImGui::TableSetColumnIndex(1);
				const auto& relativePath = std::filesystem::relative(m_directoryPath, projectDirectory).generic_string();
				ImGui::AlignTextToFramePadding();
				ImGui::Text(relativePath.c_str());
			}

			ImGui::TableSetColumnIndex(2);

			if (m_thumbnailSize <= 0)
				m_thumbnailSize = 6 * fontSize;

			ImGui::PushItemWidth(-std::numeric_limits<float>().min());
			ImGui::SliderFloat("##ThumbnailSize", &m_thumbnailSize, minThumnailSize, 10 * fontSize, "", ImGuiSliderFlags_NoInput);
			ImGui::PopItemWidth();

			ImGui::EndTable();
		}

		const int IMGUI_TABLE_MAX_COLUMNS = 64; // it is defined in imgui_internal.h
		const int columns = std::clamp(int(availableWidth / m_thumbnailSize), 1, IMGUI_TABLE_MAX_COLUMNS);

		ImGui::PushStyleColor(ImGuiCol_Button, { 0, 0, 0, 0 });
		
		if (ImGui::BeginTable("##Files", columns, ImGuiTableFlags_NoPadOuterX | ImGuiTableFlags_NoPadInnerX, tableSize)) {
			for (int i = 0; i < columns; ++i)
				ImGui::TableSetupColumn(nullptr, ImGuiTableColumnFlags_WidthFixed, m_thumbnailSize);

			const ImVec2 button_size(m_thumbnailSize - 2 * style.FramePadding.x, m_thumbnailSize - 2 * style.FramePadding.y);

			int i = 0;

			for (const auto& directory_entry : std::filesystem::directory_iterator(m_directoryPath)) {
				const auto& filePath = directory_entry.path();
				const auto& filename = filePath.filename().string();
				const auto& relativePath = std::filesystem::relative(filePath, projectDirectory);

				// ignore hidden and reserved files
				if (filename.front() == '.' || project.isPathReserved(relativePath))
					continue;
				
				const int column = i % columns;
				
				if (column == 0)
					ImGui::TableNextRow();

				ImGui::TableSetColumnIndex(column);

				if (directory_entry.is_directory()) {
					ImGui::ImageButton(filename.c_str(), (void*)(intptr_t)m_icons["Folder"]->rendererId(), button_size, { 0, 1 }, { 1, 0 });

					if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
						m_directoryPath = filePath;
				}
				else {
					ImGui::ImageButton(filename.c_str(), (void*)(intptr_t)m_icons["File"]->rendererId(), button_size, { 0, 1 }, { 1, 0 });
					const auto& buttonRectMax = ImGui::GetItemRectMax();
					const ImVec2 bottomRight(buttonRectMax.x - style.FramePadding.x, buttonRectMax.y - style.FramePadding.y);
					
					if (!project.contains(relativePath)) {
						const float notIncludedIconSize = minThumnailSize / 2;
						const ImVec2 topLeft(bottomRight.x - notIncludedIconSize, bottomRight.y - notIncludedIconSize);
						ImGui::GetWindowDrawList()->AddImage((void*)(intptr_t)m_icons["NotIncluded"]->rendererId(), topLeft, bottomRight, { 0, 1 }, { 1, 0 });
					}

					if (ImGui::IsItemHovered() && ImGui::IsItemClicked(ImGuiPopupFlags_MouseButtonRight)) {
						m_contextFile = relativePath;
						ImGui::OpenPopup("Context Menu");
					}
				}

				ImGui::TextWrapped(filename.c_str());

				++i;
			}

			if (ImGui::BeginPopup("Context Menu")) {
				const bool isScript = project.scripts().contains(m_contextFile);
				const bool isScene = project.scenes().contains(m_contextFile);

				if (isScript) {
					if (ImGui::Selectable("Exclude From Project"))
						project.removeScripts({ m_contextFile });
				}
				else if (isScene) {
					if (ImGui::Selectable("Exclude From Project"))
						project.removeScene(m_contextFile);
				}
				else {
					const auto& extension = m_contextFile.extension().string();
					if ((extension == ".h" || extension == ".hpp" || extension == ".cpp") && ImGui::Selectable("Include In Project"))
						project.addScripts({ m_contextFile });
					else if (extension == ".scene" && ImGui::Selectable("Include In Project"))
						project.addScene(m_contextFile);
				}

				if (ImGui::Selectable("Delete")) {
					if (isScript)
						project.removeScripts({ m_contextFile });
					else if (isScene)
						project.removeScene(m_contextFile);

					try {
						std::filesystem::remove(projectDirectory / m_contextFile);
					}
					catch (const std::exception& ex) {
						std::cerr << ex.what() << std::endl;
					}
				}

				ImGui::EndPopup();
			}

			ImGui::EndTable();
		}

		ImGui::PopStyleColor();
	}
}
