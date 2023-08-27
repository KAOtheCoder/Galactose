#include "AssetExplorer.h"
#include "EditorContext.h"
#include "Widgets/InputString.h"

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

	void AssetExplorer::rename() {
		if (!m_renameString.empty()) {
			const auto& file = *m_selectedFiles.begin();
			const std::filesystem::path oldPath = m_directoryPath / file;
			const std::filesystem::path newPath = m_directoryPath / (m_renameString + file.extension().string());

			try {
				if (oldPath != newPath && !std::filesystem::exists(newPath)) {				
				
					std::filesystem::rename(oldPath, newPath);
					auto& project = m_editorContext->project();
					const auto& relativePath = std::filesystem::relative(newPath, project.directory());

					project.renameScript(file, relativePath);
					project.renameScene(file, relativePath);
				
					if (project.editorScene() == file)
						project.setEditorScene(relativePath);
				}
			}
			catch (const std::exception& ex) {
				std::cerr << ex.what() << std::endl;
			}
		}

		m_renaming = false;
	}

	void AssetExplorer::clearSelection() {
		if (!m_selectedFiles.empty()) {
			if (m_renaming)
				rename();

			m_selectedFiles.clear();
		}
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

		if (ImGui::BeginTable("##Controls", 3, ImGuiTableFlags_NoPadOuterX, tableSize)) {
			const float buttonSize = fontSize + 2 * style.FramePadding.x;

			ImGui::TableSetupColumn(nullptr, ImGuiTableColumnFlags_WidthFixed, buttonSize);
			ImGui::TableSetupColumn(nullptr, ImGuiTableColumnFlags_WidthStretch, 1);
			ImGui::TableSetupColumn(nullptr, ImGuiTableColumnFlags_WidthFixed, 6 * buttonSize);

			ImGui::TableNextRow();

			if (m_directoryPath != projectDirectory) {
				ImGui::TableSetColumnIndex(0);

				if (ImGui::ImageButton("Up", (void*)(intptr_t)m_icons["Up"]->rendererId(), { fontSize, fontSize }, { 0, 1 }, { 1, 0 })) {
					clearSelection();
					m_directoryPath = m_directoryPath.parent_path();
				}

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

				const bool selected = m_selectedFiles.contains(relativePath);

				if (selected)
					ImGui::PushStyleColor(ImGuiCol_Button, style.Colors[ImGuiCol_ButtonActive]);

				if (directory_entry.is_directory()) {
					ImGui::ImageButton(filename.c_str(), (void*)(intptr_t)m_icons["Folder"]->rendererId(), button_size, { 0, 1 }, { 1, 0 });

					if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
						m_directoryPath = filePath;
				}
				else {
					if (ImGui::ImageButton(filename.c_str(), (void*)(intptr_t)m_icons["File"]->rendererId(), button_size, { 0, 1 }, { 1, 0 })) {
						if (ImGui::GetIO().KeyCtrl) {
							if (m_selectedFiles.contains(relativePath))
								m_selectedFiles.erase(relativePath);
							else
								m_selectedFiles.insert(relativePath);
						}
						else {
							m_selectedFiles = { relativePath };
						}
					}

					const auto& buttonRectMax = ImGui::GetItemRectMax();
					const ImVec2 bottomRight(buttonRectMax.x - style.FramePadding.x, buttonRectMax.y - style.FramePadding.y);
					
					if (!project.contains(relativePath)) {
						const float notIncludedIconSize = minThumnailSize / 2;
						const ImVec2 topLeft(bottomRight.x - notIncludedIconSize, bottomRight.y - notIncludedIconSize);
						ImGui::GetWindowDrawList()->AddImage((void*)(intptr_t)m_icons["NotIncluded"]->rendererId(), topLeft, bottomRight, { 0, 1 }, { 1, 0 });
					}

					if (ImGui::IsItemHovered() && ImGui::IsItemClicked(ImGuiPopupFlags_MouseButtonRight)) {
						if (!m_selectedFiles.contains(relativePath))
							m_selectedFiles = { relativePath };

						ImGui::OpenPopup("Context Menu");
					}
				}

				if (selected)
					ImGui::PopStyleColor();

				if (m_renaming && selected) {
					if (m_renameString.empty()) {
						m_renameString = relativePath.stem().string();
						ImGui::SetKeyboardFocusHere();
					}

					if (InputString::inputText("##" + filename, m_renameString))
						m_renameString = InputString::text();

					if (ImGui::IsItemDeactivated()) // focus out or press "Enter"
						rename();
				}
				else {
					ImGui::TextWrapped(filename.c_str());
				}

				++i;
			}

			if (ImGui::BeginPopup("Context Menu")) {
				std::vector<std::filesystem::path> scripts;
				std::vector<std::filesystem::path> scenes;
				std::vector<std::filesystem::path> others;

				for (const auto& file : m_selectedFiles) {
					if (project.scripts().contains(file))
						scripts.push_back(file);
					else if (project.scenes().contains(file))
						scenes.push_back(file);
					else
						others.push_back(file);
				}

				if (others.empty()) {
					if (ImGui::Selectable("Exclude From Project")) {
						project.removeScripts(scripts);

						for (const auto& file : scenes)
							project.removeScene(file);
					}
				}
				else if (scripts.empty() && scenes.empty()) {
					for (const auto& file : others) {
						const auto& extension = file.extension().string();

						if ((extension == ".h" || extension == ".hpp" || extension == ".cpp"))
							scripts.push_back(file);
						else if (extension == ".scene")
							scenes.push_back(file);
					}

					if (scripts.size() + scenes.size() == others.size() && ImGui::Selectable("Include In Project")) {
						project.addScripts(scripts);

						for (const auto& file : scenes)
							project.addScene(file);
					}

					scripts.clear();
					scenes.clear();
				}

				if (ImGui::Selectable("Delete")) {
					project.removeScripts(scripts);
					
					for (const auto& file : scenes)
						project.removeScene(file);

					try {
						for (const auto& file : m_selectedFiles)
							std::filesystem::remove(projectDirectory / file);
					}
					catch (const std::exception& ex) {
						std::cerr << ex.what() << std::endl;
					}
				}

				if (m_selectedFiles.size() == 1 && ImGui::Selectable("Rename")) {
					m_renameString.clear(); // means renaming just started, it will be used to focus on input text
					m_renaming = true;
				}

				ImGui::EndPopup();
			}

			ImGui::EndTable();
		}

		ImGui::PopStyleColor();

		if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered()
			&& (ImGui::IsMouseClicked(ImGuiMouseButton_Left) || ImGui::IsMouseClicked(ImGuiMouseButton_Right)))
		{
			clearSelection();
		}
	}
}
