#include "PropertyTable.h"
#include "FileDialog.h"

#include <Galactose/Renderer/Texture.h>

#include <imgui.h>

using namespace Galactose;

namespace GalactoseEditor {
	void PropertyTable::init() {
		s_icons.emplace("Clear", Texture::create("assets/textures/Clear.png"));
		s_icons.emplace("Folder", Texture::create("assets/textures/Folder.png"));
	}

	bool PropertyTable::beginTable(const char* a_name) {
		return ImGui::BeginTable(a_name, 2, ImGuiTableFlags_SizingStretchProp);
	}

	void PropertyTable::endTable() {
		ImGui::EndTable();
	}

	void PropertyTable::label(const char* a_label) {
		ImGui::TableNextRow();

		ImGui::TableSetColumnIndex(0);
		ImGui::AlignTextToFramePadding();
		ImGui::Text(a_label);
	}

	bool PropertyTable::iconButton(const char* a_icon) {
		const float font_size = ImGui::GetFontSize();
		return ImGui::ImageButton(a_icon, (void*)(intptr_t)s_icons[a_icon]->rendererId(), { font_size, font_size }, { 0, 1 }, { 1, 0 });
	}

	bool PropertyTable::fileInput(const char* a_label, std::filesystem::path& a_path, 
		const std::vector<FileDialog::Filter>& a_filters, const std::string& a_emptyText) 
	{
		label(a_label);

		ImGui::TableSetColumnIndex(1);
		bool changed = false;

		if (ImGui::BeginTable(a_label, 3, ImGuiTableFlags_NoPadInnerX | ImGuiTableFlags_SizingStretchProp)) {
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);

			std::string input_text = a_path.empty() ? a_emptyText : a_path.stem().generic_string();
			const auto& input_label = std::string("##") + a_label;
			ImGui::PushItemWidth(-std::numeric_limits<float>().min());
			ImGui::InputText(input_label.c_str(), input_text.data(), input_text.size(), ImGuiInputTextFlags_ReadOnly);
			ImGui::PopItemWidth();

			ImGui::TableSetColumnIndex(1);

			if (iconButton("Clear") && !a_path.empty()) {
				a_path = "";
				changed = true;
			}

			ImGui::TableSetColumnIndex(2);
			if (iconButton("Folder")) {
				const auto& path = FileDialog::open(a_filters);

				if (a_path != path) {
					a_path = path;
					changed = true;
				}
			}

			ImGui::EndTable();
		}

		return changed;
	}
}
