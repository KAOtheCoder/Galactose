#include "Inspector.h"
#include "EditorSceneData.h"
#include "InputString.h"

#include <Scene/Components/Transform.h>

#include <imgui.h>
#include <imgui_internal.h>

using namespace Galactose;

namespace GalactoseEditor {
	Inspector::Inspector(const std::shared_ptr<EditorSceneData>& a_sceneData) :
		Panel("Inspector"),
		m_sceneData(a_sceneData)
	{}

	void Inspector::onUpdate() {
		auto entity = m_sceneData->selectedEntity();

		if (!entity)
			return;
		
		if (InputString::inputText("##Name", entity->name()))
			entity->setName(InputString::text());

		ImGui::Separator();
		drawTransform();
	}

	bool Inspector::dragVector3Axis(const int a_axis, Vector3& a_value) {
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

		const int column = 3 * a_axis + 2;
		ImGui::TableSetColumnIndex(column);

		const std::string label(1, 'X' + a_axis);
		const bool clicked = ImGui::Button(label.c_str());

		ImGui::PopStyleColor(3);

		ImGui::TableSetColumnIndex(column + 1);
		ImGui::PushItemWidth(-1);
		const auto& input_label = "##" + label;
		const bool changed = ImGui::DragFloat(input_label.c_str(), &a_value[a_axis], 0.1f);
		ImGui::PopItemWidth();

		if (clicked)
			ImGui::SetKeyboardFocusHere(-1);

		return changed;
	}

	bool Inspector::dragVector3(const char* a_label, Vector3& a_value) {
		ImGui::PushID(a_label);
		ImGui::TableNextRow();

		ImGui::TableSetColumnIndex(0);
		ImGui::Text(a_label);

		const bool xChanged = dragVector3Axis(0, a_value);
		const bool yChanged = dragVector3Axis(1, a_value);
		const bool zChanged = dragVector3Axis(2, a_value);

		ImGui::PopID();

		return xChanged || yChanged || zChanged;
	}

	void Inspector::drawTransform() {
		auto transform = m_sceneData->selectedEntity()->getTransform();

		const float padding = 2 * ImGui::GetStyle().CellPadding.x;

		if (ImGui::BeginTable("Transform", 10, ImGuiTableFlags_NoPadInnerX)) {
			ImGui::TableSetupColumn("#Label", ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableSetupColumn("#XPadding", ImGuiTableColumnFlags_WidthFixed, padding);
			ImGui::TableSetupColumn("#X", ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableSetupColumn("#XInput", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn("#YPadding", ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableSetupColumn("#Y", ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableSetupColumn("#YInput", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn("#ZPadding", ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableSetupColumn("#Z", ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableSetupColumn("#ZInput", ImGuiTableColumnFlags_WidthStretch);

			auto position = transform->position();
			if (dragVector3("Position", position))
				transform->setPosition(position);

			auto rotation = transform->rotation().eulerDegrees();
			if (dragVector3("Rotation", rotation))
				transform->setRotation(Quaternion::fromEulerDegrees(rotation));

			auto scale = transform->scale();
			if (dragVector3("Scale", scale))
				transform->setScale(scale);

			ImGui::EndTable();
		}
	}
}
