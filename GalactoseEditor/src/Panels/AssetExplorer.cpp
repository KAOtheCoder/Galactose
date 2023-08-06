#include "AssetExplorer.h"
#include "EditorContext.h"

#include <Galactose/Renderer/Texture.h>

using namespace Galactose;

namespace GalactoseEditor {
	AssetExplorer::AssetExplorer(const std::shared_ptr<EditorContext>& a_editorContext) : 
		Panel("Asset Explorer"),
		m_editorContext(a_editorContext),
		m_path(a_editorContext->project().directory())
	{
		m_icons.emplace("Up", Texture::create("assets/textures/Up.png"));
		m_icons.emplace("Folder", Texture::create("assets/textures/FolderL.png"));
		m_icons.emplace("File", Texture::create("assets/textures/FileL.png"));
	}

	void AssetExplorer::onUpdate() {
		const auto& style = ImGui::GetStyle();
		// always ignore vertical scrollbar area to avoid flickering 
		const auto available_width = ImGui::GetContentRegionAvail().x - (ImGui::GetScrollMaxY() > 0 ? 0 : style.ScrollbarSize);
		const ImVec2 table_size(available_width, 0);

		if (ImGui::BeginTable("##Controls", 3, ImGuiTableFlags_NoPadOuterX, table_size)) {
			const float font_size = ImGui::GetFontSize();
			const float button_size = font_size + 2 * style.FramePadding.x;

			ImGui::TableSetupColumn(nullptr, ImGuiTableColumnFlags_WidthFixed, button_size);
			ImGui::TableSetupColumn(nullptr, ImGuiTableColumnFlags_WidthStretch, 1);
			ImGui::TableSetupColumn(nullptr, ImGuiTableColumnFlags_WidthFixed, 6 * button_size);

			ImGui::TableNextRow();

			if (m_path != m_editorContext->project().directory()) {
				ImGui::TableSetColumnIndex(0);

				if (ImGui::ImageButton("Up", (void*)(intptr_t)m_icons["Up"]->rendererId(), { font_size, font_size }, { 0, 1 }, { 1, 0 }))
					m_path = m_path.parent_path();
			}

			ImGui::TableSetColumnIndex(1);
			const auto& currentPath = m_path.generic_string();
			ImGui::AlignTextToFramePadding();
			ImGui::Text(currentPath.c_str());

			ImGui::TableSetColumnIndex(2);

			if (m_thumbnailSize <= 0)
				m_thumbnailSize = 6 * font_size;

			ImGui::PushItemWidth(-std::numeric_limits<float>().min());
			ImGui::SliderFloat("##ThumbnailSize", &m_thumbnailSize, 2 * font_size, 10 * font_size, "", ImGuiSliderFlags_NoInput);
			ImGui::PopItemWidth();

			ImGui::EndTable();
		}

		const int IMGUI_TABLE_MAX_COLUMNS = 64; // it is defined in imgui_internal.h
		const int columns = std::clamp(int(available_width / m_thumbnailSize), 1, IMGUI_TABLE_MAX_COLUMNS);

		ImGui::PushStyleColor(ImGuiCol_Button, { 0, 0, 0, 0 });
		
		if (ImGui::BeginTable("##Files", columns, ImGuiTableFlags_NoPadOuterX | ImGuiTableFlags_NoPadInnerX, table_size)) {
			for (int i = 0; i < columns; ++i)
				ImGui::TableSetupColumn(nullptr, ImGuiTableColumnFlags_WidthFixed, m_thumbnailSize);

			const ImVec2 button_size(m_thumbnailSize - 2 * style.FramePadding.x, m_thumbnailSize - 2 * style.FramePadding.y);

			int i = 0;

			for (const auto& directory_entry : std::filesystem::directory_iterator(m_path)) {
				const int column = i % columns;
				
				if (column == 0)
					ImGui::TableNextRow();

				ImGui::TableSetColumnIndex(column);

				const bool isDirectory = directory_entry.is_directory();
				const auto& file_path = directory_entry.path();
				const auto& filename = file_path.filename().string();

				ImGui::ImageButton(filename.c_str(), (void*)(intptr_t)m_icons[isDirectory ? "Folder" : "File"]->rendererId(), button_size, {0, 1}, {1, 0});
				
				if (isDirectory && ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
					m_path = file_path;

				ImGui::TextWrapped(filename.c_str());

				++i;
			}

			ImGui::EndTable();
		}

		ImGui::PopStyleColor();
	}
}
