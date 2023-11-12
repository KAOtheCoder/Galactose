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
				PropertyTable::openPopup("Add Component");

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

		if (PropertyTable::dragVector(property->name().c_str(), value.length(), value.data()))
			accessibleProperty->set(value);
	}

	void Inspector::drawScript(Script* a_script) {
		if (drawComponentHeader(a_script, a_script->name().c_str()) && PropertyTable::beginTable(a_script->name().c_str())) {
			for (auto property : a_script->properties()) {	
				const bool readOnly = property->isReadOnly();

				if (readOnly)
					ImGui::BeginDisabled();

				switch (property->type()) {
				case DataType::Bool: {
					auto accessibleProperty = static_cast<AccessibleProperty<bool>*>(property);
					auto value = accessibleProperty->get();
					if (PropertyTable::checkBox(property->name().c_str(), value))
						accessibleProperty->set(value);

					break;
				}
				case DataType::Float: {
					auto accessibleProperty = static_cast<AccessibleProperty<float>*>(property);
					auto value = accessibleProperty->get();
					
					if (property->hasLimits()) {
						if (PropertyTable::dragFloat(property->name().c_str(), value, accessibleProperty->speed(), accessibleProperty->min(), accessibleProperty->max()))
							accessibleProperty->set(value);
					}
					else {
						if (PropertyTable::dragFloat(property->name().c_str(), value))
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
