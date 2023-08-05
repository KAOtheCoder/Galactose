#include "AssetExplorer.h"
#include "EditorContext.h"

#include <Galactose/Renderer/Texture.h>

#include <imgui_internal.h>

using namespace Galactose;

namespace GalactoseEditor {
	AssetExplorer::AssetExplorer(const std::shared_ptr<EditorContext>& a_editorContext) : 
		Panel("Asset Explorer"),
		m_editorContext(a_editorContext),
		m_path(a_editorContext->project().directory())
	{
		m_icons.emplace("Folder", Texture::create("assets/textures/Folder.png"));
	}

	void AssetExplorer::onUpdate() {
		if (ImGui::BeginTable("##Controls", 3)) {
			ImGui::TableSetupColumn(nullptr, ImGuiTableColumnFlags_WidthStretch, 1);
			ImGui::TableSetupColumn(nullptr, ImGuiTableColumnFlags_WidthStretch, 2); //empty stretch
			ImGui::TableSetupColumn(nullptr, ImGuiTableColumnFlags_WidthStretch, 1);

			ImGui::TableNextRow();

			if (m_path != m_editorContext->project().directory()) {
				ImGui::TableSetColumnIndex(0);

				if (ImGui::Button("Back"))
					m_path = m_path.parent_path();
			}

			ImGui::TableSetColumnIndex(2);
			const float font_size = ImGui::GetFontSize();

			if (m_thumbnailSize <= 0)
				m_thumbnailSize = 6 * font_size;

			ImGui::PushItemWidth(-std::numeric_limits<float>().min());
			ImGui::SliderFloat("##ThumbnailSize", &m_thumbnailSize, 2 * font_size, 10 * font_size, "", ImGuiSliderFlags_NoInput);
			ImGui::PopItemWidth();

			ImGui::EndTable();
		}
		
		const auto& style = ImGui::GetStyle();
		// always ignore vertical scrollbar area to avoid flickering 
		const auto available_width = ImGui::GetContentRegionAvail().x - (ImGui::GetScrollMaxY() > 0 ? 0 : style.ScrollbarSize);
		const int columns = std::clamp(int(available_width / m_thumbnailSize), 1, IMGUI_TABLE_MAX_COLUMNS);

		ImGui::PushStyleColor(ImGuiCol_Button, { 0, 0, 0, 0 });
		
		if (ImGui::BeginTable("##Files", columns, ImGuiTableFlags_NoPadOuterX | ImGuiTableFlags_NoPadInnerX)) {
			for (int i = 0; i < columns; ++i)
				ImGui::TableSetupColumn(nullptr, ImGuiTableColumnFlags_WidthFixed, m_thumbnailSize);

			const ImVec2 button_size(m_thumbnailSize - 2 * style.FramePadding.x, m_thumbnailSize - 2 * style.FramePadding.y);

			int i = 0;

			for (const auto& directory_entry : std::filesystem::directory_iterator(m_path)) {
				const int column = i % columns;
				
				if (column == 0)
					ImGui::TableNextRow();

				ImGui::TableSetColumnIndex(column);

				const auto& file_path = directory_entry.path();
				const auto& filename = file_path.filename().string();

				ImGui::ImageButton(filename.c_str(), (void*)(intptr_t)m_icons["Folder"]->rendererId(), button_size, { 0, 1 }, { 1, 0 });
				
				if (directory_entry.is_directory() && ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
					m_path = file_path;

				ImGui::TextWrapped(filename.c_str());

				++i;
			}

			ImGui::EndTable();
		}

		ImGui::PopStyleColor();
	}
}
