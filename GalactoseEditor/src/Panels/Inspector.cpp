#include "Inspector.h"
#include "EditorSceneData.h"
#include "InputString.h"

#include <Scene/Components/Transform.h>
#include <Scene/Components/SpriteRenderer.h>
#include <Renderer/Texture.h>

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

		drawTransform();
		drawSpriteRenderer();
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

	bool Inspector::drawComponentHeader(const char* a_label) {
		return ImGui::CollapsingHeader(a_label, ImGuiTreeNodeFlags_DefaultOpen);
	}

	void Inspector::drawTransform() {
		if (!drawComponentHeader("Transform"))
			return;

		auto transform = m_sceneData->selectedEntity()->getTransform();

		const float padding = 2 * ImGui::GetStyle().CellPadding.x;

		if (ImGui::BeginTable("Transform", 2, ImGuiTableFlags_SizingStretchProp)) {
			auto position = transform->localPosition();
			if (dragVector("Position", 3, position.data()))
				transform->setLocalPosition(position);

			auto rotation = transform->localRotation().eulerDegrees();
			if (dragVector("Rotation", 3, rotation.data()))
				transform->setLocalRotation(Quaternion::fromEulerDegrees(rotation));

			auto scale = transform->localScale();
			if (dragVector("Scale", 3, scale.data()))
				transform->setLocalScale(scale);

			ImGui::EndTable();
		}
	}

	void Inspector::drawSpriteRenderer() {
		if (!m_sceneData->selectedEntity()->hasComponent<SpriteRenderer>())
			return;

		if (!drawComponentHeader("Sprite Renderer"))
			return;

		auto spriteRenderer = m_sceneData->selectedEntity()->getComponent<SpriteRenderer>();
		auto& sprite = spriteRenderer->sprite;

		if (ImGui::BeginTable("SpriteRenderer", 2, ImGuiTableFlags_SizingStretchProp)) {
			// TO DO: set texture
			drawLabel("Texture");

			ImGui::TableSetColumnIndex(1);
			auto texture = sprite.texture();
			std::string textureName = texture ? texture->filePath() : "Default";
			ImGui::PushItemWidth(-std::numeric_limits<float>().min());
			ImGui::InputText("##Texture", textureName.data(), textureName.size(), ImGuiInputTextFlags_ReadOnly);
			ImGui::PopItemWidth();

			// TO DO: stretch color button
			drawLabel("Color");

			ImGui::TableSetColumnIndex(1);

			auto color = sprite.color();
			if (ImGui::ColorEdit4("##Color", color.data(), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel))
				sprite.setColor(color);

			auto size = sprite.size();
			if (dragVector("Size", 2, size.data()))
				sprite.setSize(size);

			auto pivot = sprite.pivot();
			if (dragVector("Pivot", 2, pivot.data()))
				sprite.setPivot(pivot);

			ImGui::EndTable();
		}
	}
}
