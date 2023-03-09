#include "Inspector.h"
#include "Panels/InputString.h"

#include <Core/Global.h>
#include <Renderer/Texture.h>
#include <Scene/Components/Transform.h>

#include <imgui.h>
#include <imgui_internal.h>

#include <nfd.hpp>

#include <filesystem>

using namespace Galactose;

namespace GalactoseEditor {
	Inspector::Inspector(const std::shared_ptr<EditorSceneData>& a_sceneData) :
		Panel("Inspector"),
		m_sceneData(a_sceneData)
	{
		m_icons.emplace("clear", Galactose::Texture::create("assets/textures/clear.png"));
		m_icons.emplace("folder", Galactose::Texture::create("assets/textures/folder.png"));

		bindComponentDrawer<Transform>();
		bindComponentDrawer<SpriteRenderer>();
	}

	void Inspector::onUpdate() {
		auto entity = m_sceneData->selectedEntity();

		if (!entity)
			return;
		
		if (InputString::inputText("##Name", entity->name()))
			entity->setName(InputString::text());

		for (auto component : entity->components()) {
			auto iter = m_componentDrawers.find(component->type());
			GT_ASSERT(iter != m_componentDrawers.end(), "No drawer assigned for component '" + component->name() + "'.");
			(this->*iter->second)();
		}

		ImGui::Separator();
	}

	void Inspector::drawLabel(const char* a_label) {
		ImGui::TableNextRow();

		ImGui::TableSetColumnIndex(0);
		ImGui::AlignTextToFramePadding();
		ImGui::Text(a_label);
	}

	bool Inspector::dragVector3Axis(const int a_axis, float& a_value) {
		const float AXIS_INTENSITY = 0.6f;
		const float INTENSITY = 0.1f;
		const float ALPHA = 1.0f;
		const Vector3 HOVER_COLOR = { 0.1f, 0.1f, 0.1f};

		Vector3 color(INTENSITY, INTENSITY, INTENSITY);
		color[a_axis] = AXIS_INTENSITY;
		Vector3 hoverColor = color + HOVER_COLOR;
		const ImVec4 buttonColor(color.x, color.y, color.z, ALPHA);
		ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { hoverColor.x, hoverColor.y, hoverColor.z, ALPHA });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, buttonColor);

		const int column = 2 * a_axis;
		ImGui::TableSetColumnIndex(column);

		const std::string label(1, 'X' + a_axis);
		const bool clicked = ImGui::Button(label.c_str());

		ImGui::PopStyleColor(3);

		ImGui::TableSetColumnIndex(column + 1);
		ImGui::PushItemWidth(-std::numeric_limits<float>().min());
		const auto& input_label = "##" + label;
		const bool changed = ImGui::DragFloat(input_label.c_str(), &a_value, 0.1f);
		ImGui::PopItemWidth();

		if (clicked)
			ImGui::SetKeyboardFocusHere(-1); // focus next item

		return changed;
	}

	bool Inspector::dragVector(const char* a_label, const int a_axisCount, float* a_value) {
		drawLabel(a_label);

		ImGui::TableSetColumnIndex(1);
		if (ImGui::BeginTable(a_label, 2 * a_axisCount, ImGuiTableFlags_NoPadInnerX)) {
			for (int i = 0; i < a_axisCount; ++i) {
				ImGui::TableSetupColumn(nullptr, ImGuiTableColumnFlags_WidthFixed);
				ImGui::TableSetupColumn(nullptr, ImGuiTableColumnFlags_WidthStretch);
			}

			ImGui::TableNextRow();

			bool changed = false;
			for (int i = 0; i < a_axisCount; ++i)
				changed = dragVector3Axis(i, a_value[i]) || changed;

			ImGui::EndTable();

			return changed;
		}

		return false;
	}

	bool Inspector::colorButton(const char* a_label, Vector4& a_color) {
		drawLabel(a_label);

		ImGui::TableSetColumnIndex(1);

		bool changed = false;
		const ImVec4 color(a_color.r, a_color.g, a_color.b, a_color.a);
		auto context = ImGui::GetCurrentContext();
		
		if (ImGui::ColorButton(a_label, color, 0, { ImGui::GetContentRegionAvail().x, 0 })) {
			context->ColorPickerRef = color;
			ImGui::OpenPopup("picker");
			const auto& bottomLeft = context->LastItemData.Rect.GetBL();
			ImGui::SetNextWindowPos({ bottomLeft.x, bottomLeft.y + ImGui::GetStyle().ItemSpacing.y });
		}

		if (ImGui::BeginPopup("picker"))
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

	bool Inspector::iconButton(const char* a_icon) {
		const float font_size = ImGui::GetFontSize();
		return ImGui::ImageButton(a_icon, (void*)(intptr_t)m_icons[a_icon]->rendererId(), { font_size, font_size }, { 0, 1 }, { 1, 0 });
	}

	bool Inspector::drawFileInput(const char* a_label, std::string& a_path, const std::string& a_emptyText) {
		drawLabel(a_label);

		ImGui::TableSetColumnIndex(1);
		bool changed = false;

		if (ImGui::BeginTable(a_label, 3, ImGuiTableFlags_NoPadInnerX | ImGuiTableFlags_SizingStretchProp)) {
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);

			auto input_text = a_path.empty() ? a_emptyText : std::filesystem::path(a_path).stem().generic_string();
			const auto& input_label = std::string("##") + a_label;
			ImGui::PushItemWidth(-std::numeric_limits<float>().min());
			ImGui::InputText(input_label.c_str(), input_text.data(), input_text.size(), ImGuiInputTextFlags_ReadOnly);
			ImGui::PopItemWidth();

			ImGui::TableSetColumnIndex(1);
			
			if (iconButton("clear") && !a_path.empty()) {
				a_path = "";
				changed = true;
			}

			ImGui::TableSetColumnIndex(2);
			if (iconButton("folder")) {
				nfdchar_t* path;
				nfdfilteritem_t filter = { "Texture", "png" };
				const auto result = NFD_OpenDialog(&path, &filter, 1, nullptr);
				GT_ASSERT(result != NFD_ERROR, NFD_GetError());
				
				if (result == NFD_OKAY) {
					a_path = path;
					changed = true;
				}
			}

			ImGui::EndTable();
		}

		return changed;
	}

	bool Inspector::drawComponentHeader(const char* a_label) {
		return ImGui::CollapsingHeader(a_label, ImGuiTreeNodeFlags_DefaultOpen);
	}
}
