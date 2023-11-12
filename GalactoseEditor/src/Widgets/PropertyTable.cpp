#include "PropertyTable.h"
#include "FileDialog.h"

#include <Galactose/Renderer/Texture.h>

#include <imgui.h>
#include <imgui_internal.h>

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

	void PropertyTable::openPopup(const char* a_label) {
		ImGui::OpenPopup(a_label);
		const auto& bottomRight = ImGui::GetCurrentContext()->LastItemData.Rect.GetBR();
		ImGui::SetNextWindowPos({ bottomRight.x, bottomRight.y + ImGui::GetStyle().ItemSpacing.y }, 0, { 1, 0 });
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

	bool PropertyTable::sliderFloat(const char* a_label, float& a_value, const float a_min, const float a_max) {
		label(a_label);

		ImGui::TableSetColumnIndex(1);

		ImGui::PushItemWidth(-std::numeric_limits<float>().min());
		const auto& id = std::string("##") + a_label;
		const bool changed = ImGui::SliderFloat(id.c_str(), &a_value, a_min, a_max, "%.3f", ImGuiSliderFlags_AlwaysClamp);
		ImGui::PopItemWidth();

		return changed;
	}

	bool PropertyTable::spanDragFloat(const char* a_label, float& a_value, const float a_speed, const float a_min, const float a_max) {
		ImGui::PushItemWidth(-std::numeric_limits<float>().min());
		const auto& id = std::string("##") + a_label;
		const auto changed = ImGui::DragFloat(id.c_str(), &a_value, a_speed, a_min, a_max, "%.3f", ImGuiSliderFlags_AlwaysClamp);
		ImGui::PopItemWidth();

		return changed;
	}

	bool PropertyTable::dragFloat(const char* a_label, float& a_value, const float a_speed, const float a_min, const float a_max) {
		PropertyTable::label(a_label);

		ImGui::TableSetColumnIndex(1);
		return spanDragFloat(a_label, a_value, a_speed, a_min, a_max);
	}

	bool PropertyTable::colorButton(const char* a_label, Vector4& a_color) {
		PropertyTable::label(a_label);

		ImGui::TableSetColumnIndex(1);

		bool changed = false;
		const ImVec4 color(a_color.r, a_color.g, a_color.b, a_color.a);
		auto context = ImGui::GetCurrentContext();

		if (ImGui::ColorButton(a_label, color, 0, { ImGui::GetContentRegionAvail().x, 0 })) {
			context->ColorPickerRef = color;
			openPopup("Color Picker");
		}

		if (ImGui::BeginPopup("Color Picker"))
		{
			if (ImGui::GetCurrentWindow()->BeginCount == 1)
			{
				const char* labelEnd = ImGui::FindRenderedTextEnd(a_label);
				if (a_label != labelEnd)
				{
					ImGui::TextEx(a_label, labelEnd);
					ImGui::Spacing();
				}

				const ImGuiColorEditFlags flags = ImGuiColorEditFlags_DisplayMask_ | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaPreviewHalf;
				changed = ImGui::ColorPicker4("Current", a_color.data(), flags, &context->ColorPickerRef.x);
			}

			ImGui::EndPopup();
		}

		return changed;
	}

	bool PropertyTable::dragVectorAxis(const int a_axis, float& a_value, const Galactose::Vector4& color, const Galactose::Vector4& hoverColor) {
		const ImVec4 buttonColor(color.x, color.y, color.z, color.a);
		ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { hoverColor.x, hoverColor.y, hoverColor.z, hoverColor.a });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, buttonColor);

		const auto labels = "XYZW";
		const int column = 2 * a_axis;
		ImGui::TableSetColumnIndex(column);

		const std::string label(1, labels[a_axis]);
		const bool clicked = ImGui::Button(label.c_str());

		ImGui::PopStyleColor(3);

		ImGui::TableSetColumnIndex(column + 1);
		const bool changed = spanDragFloat(label.c_str(), a_value);

		if (clicked)
			ImGui::SetKeyboardFocusHere(-1); // focus next item

		return changed;
	}

	bool PropertyTable::dragVector(const char* a_label, const int a_axisCount, float* a_value, const bool colored) {
		label(a_label);

		ImGui::TableSetColumnIndex(1);
		if (ImGui::BeginTable(a_label, 2 * a_axisCount, ImGuiTableFlags_NoPadInnerX)) {
			for (int i = 0; i < a_axisCount; ++i) {
				ImGui::TableSetupColumn(nullptr, ImGuiTableColumnFlags_WidthFixed);
				ImGui::TableSetupColumn(nullptr, ImGuiTableColumnFlags_WidthStretch);
			}

			ImGui::TableNextRow();

			bool changed = false;

			for (int i = 0; i < a_axisCount; ++i) {
				if (colored && i < 3) {
					const float AXIS_INTENSITY = 0.6f;
					const float INTENSITY = 0.1f;
					const float ALPHA = 1.0f;
					const Vector4 HOVER_COLOR = { 0.1f, 0.1f, 0.1f, 0.f };
					Vector4 color(INTENSITY, INTENSITY, INTENSITY, ALPHA);
					color[i] = AXIS_INTENSITY;
					const auto& hoverColor = color + HOVER_COLOR;

					changed = dragVectorAxis(i, a_value[i], color, hoverColor) || changed;
				}
				else {
					changed = dragVectorAxis(i, a_value[i]) || changed;
				}
			}

			ImGui::EndTable();

			return changed;
		}

		return false;
	}

	bool PropertyTable::checkBox(const char* a_label, bool& a_value) {
		label(a_label);

		ImGui::TableSetColumnIndex(1);
		const auto& id = std::string("##") + a_label;
		return ImGui::Checkbox(id.c_str(), &a_value);
	}
}
