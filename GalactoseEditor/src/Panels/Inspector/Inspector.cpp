#include "Inspector.h"
#include "Widgets/InputString.h"
#include "Widgets/TrailingCollapsingHeader.h"
#include "Widgets/PropertyTable.h"

#include <Galactose/Core/Global.h>
#include <Galactose/Renderer/Texture.h>
#include <Galactose/Scene/Components/Transform.h>
#include <Galactose/Scene/Components/Script.h>
#include <Galactose/Scene/Components/Camera.h>
#include <Galactose/Scene/Components/SpriteRenderer.h>

#include <imgui.h>
#include <imgui_internal.h>

#include <nfd.hpp>

#include <filesystem>

using namespace Galactose;

namespace GalactoseEditor {
	Inspector::Inspector(const std::shared_ptr<EditorContext>& a_editorContext) :
		Panel("Inspector"),
		m_editorContext(a_editorContext)
	{
		bindComponent<Transform>();
		bindComponent<Camera>();
		bindComponent<SpriteRenderer>();
	}

	template <class C>
	void Inspector::drawComponentContentHelper(Component* a_component) {
		drawComponentContent<C>(static_cast<C*>(a_component));
	}

	template <class C>
	void Inspector::bindComponent() {
		m_componentInfos[C::staticType()] = {
			toReadableName(C::staticName()),
			&Inspector::drawComponentContentHelper<C>,
			&Entity::hasComponent<C>,
			&C::create
		};
	}

	void Inspector::onUpdate() {
		auto entity = m_editorContext->selectedEntity();

		if (!entity)
			return;

		if (ImGui::BeginTable("Name", 2)) {
			ImGui::TableSetupColumn(nullptr, ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn(nullptr, ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableNextRow();

			ImGui::TableSetColumnIndex(0);
			if (InputString::inputText("##Name", entity->name()))
				entity->setName(InputString::text());

			ImGui::TableSetColumnIndex(1);
			const auto frameHeight = ImGui::GetFrameHeight();

			if (ImGui::Button("+", { frameHeight, frameHeight }))
				openPopup("Add Component");

			if (ImGui::BeginPopup("Add Component")) {
				const auto transformId = Transform::staticType();

				for (const auto& [type, info] : m_componentInfos)
					if (type != transformId && ImGui::Selectable(info.name.c_str(), false, (entity->*info.has)() ? ImGuiSelectableFlags_Disabled : 0))
						(*info.create)(entity);

				const auto& scripts = Script::scripts();

				if (!scripts.empty())
					if (ImGui::BeginMenu("Script")) {
						for (const auto& script : scripts)
							if (ImGui::Selectable(toReadableName(script).c_str()))
								entity->addComponent(script);

						ImGui::EndMenu();
					}

				ImGui::EndPopup();
			}

			ImGui::EndTable();
		}

		const auto& components = entity->components();
		const auto componentCount = components.size();
		const auto scriptOffset = entity->scriptOffset();
		
		for (size_t i = 0; i < scriptOffset; ++i)
			drawComponent(components[i]);

		for (auto i = scriptOffset; i < componentCount; ++i)
			drawScript(static_cast<Script*>(components[i]));

		ImGui::Separator();

		if (m_removeComponent) {
			m_removeComponent->destroy();
			m_removeComponent = nullptr;
		}
	}

	void Inspector::openPopup(const char* a_label) {
		ImGui::OpenPopup(a_label);
		const auto& bottomRight = ImGui::GetCurrentContext()->LastItemData.Rect.GetBR();
		ImGui::SetNextWindowPos({ bottomRight.x, bottomRight.y + ImGui::GetStyle().ItemSpacing.y }, 0, { 1, 0 });
	}

	bool Inspector::dragVectorAxis(const int a_axis, float& a_value, const Galactose::Vector4& color, const Galactose::Vector4& hoverColor) {
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

	bool Inspector::dragVector(const char* a_label, const int a_axisCount, float* a_value, const bool colored) {
		PropertyTable::label(a_label);

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

	bool Inspector::spanDragFloat(const char* a_label, float& a_value, const float a_speed, const float a_min, const float a_max) {
		ImGui::PushItemWidth(-std::numeric_limits<float>().min());
		const auto& label = std::string("##") + a_label;
		const auto changed = ImGui::DragFloat(label.c_str(), &a_value, a_speed, a_min, a_max, "%.3f", ImGuiSliderFlags_AlwaysClamp);
		ImGui::PopItemWidth();

		return changed;
	}

	bool Inspector::checkBox(const char* a_label, bool& a_value) {
		PropertyTable::label(a_label);

		ImGui::TableSetColumnIndex(1);
		const auto& label = std::string("##") + a_label;
		return ImGui::Checkbox(label.c_str(), &a_value);
	}

	bool Inspector::dragFloat(const char* a_label, float& a_value, const float a_speed, const float a_min, const float a_max) {
		PropertyTable::label(a_label);

		ImGui::TableSetColumnIndex(1);
		return spanDragFloat(a_label, a_value, a_speed, a_min, a_max);
	}

	bool Inspector::colorButton(const char* a_label, Vector4& a_color) {
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

	bool Inspector::drawComponentHeader(Component* a_component, const char* a_title) {
		bool remove = false;
		bool opened = TrailingCollapsingHeader::draw(a_title, "-", remove);

		if (remove)
			m_removeComponent = a_component;

		return opened;
	}

	void Inspector::drawComponent(Component* a_component) {
		const auto type = a_component->type();
		auto iter = m_componentInfos.find(type);
		GT_ASSERT(iter != m_componentInfos.end(), "Component '" + a_component->name() + "' not binded.");
		const auto& info = iter->second;

		const bool opened = type == Transform::staticType()
			? ImGui::CollapsingHeader(info.name.c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanAvailWidth)
			: drawComponentHeader(a_component, info.name.c_str());

		if (opened && PropertyTable::beginTable(info.name.c_str())) {
			(this->*info.draw)(a_component);
			PropertyTable::endTable();
		}
	}

	template <typename T>
	void Inspector::drawVectorProperty(PropertyBase* property) {
		auto accessibleProperty = static_cast<AccessibleProperty<T>*>(property);
		auto value = accessibleProperty->get();

		if (dragVector(property->name().c_str(), value.length(), value.data()))
			accessibleProperty->set(value);
	}

	void Inspector::drawScript(Script* a_script) {
		if (drawComponentHeader(a_script, a_script->name().c_str()) && ImGui::BeginTable(a_script->name().c_str(), 2, ImGuiTableFlags_SizingStretchProp)) {
			for (auto property : a_script->properties()) {	
				const bool readOnly = property->isReadOnly();

				if (readOnly)
					ImGui::BeginDisabled();

				switch (property->type()) {
				case DataType::Bool: {
					auto accessibleProperty = static_cast<AccessibleProperty<bool>*>(property);
					auto value = accessibleProperty->get();
					if (checkBox(property->name().c_str(), value))
						accessibleProperty->set(value);

					break;
				}
				case DataType::Float: {
					auto accessibleProperty = static_cast<AccessibleProperty<float>*>(property);
					auto value = accessibleProperty->get();
					
					if (property->hasLimits()) {
						if (dragFloat(property->name().c_str(), value, accessibleProperty->speed(), accessibleProperty->min(), accessibleProperty->max()))
							accessibleProperty->set(value);
					}
					else {
						if (dragFloat(property->name().c_str(), value))
							accessibleProperty->set(value);
					}

					break;
				}
				case DataType::Vector2:
					drawVectorProperty<Vector2>(property);
					break;
				case DataType::Vector3:
					drawVectorProperty<Vector3>(property);
					break;
				case DataType::Vector4:
					drawVectorProperty<Vector4>(property);
					break;
				}

				if (readOnly)
					ImGui::EndDisabled();
			}

			ImGui::EndTable();
		}
	}

	std::string Inspector::toReadableName(const std::string& a_name) {
		GT_ASSERT(!a_name.empty(), "Component name is empty.");
		std::string readable(1, a_name.front());
		const auto nameSize = a_name.size();
		readable.reserve(nameSize);

		for (size_t i = 1; i < nameSize; ++i) {
			const auto c = a_name[i];
			if (std::isupper(c) && std::islower(a_name[i - 1]))
				readable.push_back(' ');

			readable.push_back(c);
		}

		return readable;
	}
}
