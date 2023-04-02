#include "SceneHierarchy.h"
#include "EditorSceneData.h"

#include <imgui.h>

using namespace Galactose;

namespace GalactoseEditor {
	SceneHierarchy::SceneHierarchy(const std::shared_ptr<EditorSceneData>& a_sceneData) :
		Panel("Scene Hierarchy"),
		m_sceneData(a_sceneData)
	{}

	void SceneHierarchy::drawEntityNode(Entity* a_entity) {
		const auto& children = a_entity->children();
		
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_OpenOnArrow 
			| ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;

		if (children.empty())
			flags |= ImGuiTreeNodeFlags_Leaf;

		bool selected = m_sceneData->selectedEntity() == a_entity;
		
		if (selected)
			flags |= ImGuiTreeNodeFlags_Selected;
		
		bool opened = ImGui::TreeNodeEx(a_entity, flags, a_entity->name().c_str());

		if (ImGui::IsItemClicked()) {
			m_sceneData->setSelectedEntity(a_entity);
			selected = true;
		}
		
		if (ImGui::IsItemHovered() || selected) {
			const auto frameHeight = ImGui::GetFrameHeight();
			ImGui::SameLine(ImGui::GetWindowWidth() - ImGui::GetStyle().WindowPadding.x - frameHeight);
			
			if (ImGui::Button("-", { frameHeight, frameHeight })) {
				a_entity->destroy();

				if (selected)
					m_sceneData->setSelectedEntity(nullptr);

				if (opened) {
					ImGui::TreePop();
					opened = false;
				}
			}
		}

		if (opened) {
			for (const auto child : children)
				drawEntityNode(child);

			ImGui::TreePop();
		}
	}

	void SceneHierarchy::onUpdate() {
		const auto& scene = m_sceneData->scene();
		if (scene) {
			if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsWindowHovered())
				m_sceneData->setSelectedEntity(nullptr);

			const auto& sceneName = scene->name();
			const auto safeSceneName = sceneName.empty() ? "Untitled" : sceneName.c_str();
			bool opened = false;

			if (ImGui::BeginTable(safeSceneName, 3, ImGuiTableFlags_NoPadInnerX)) {
				ImGui::TableSetupColumn(nullptr, ImGuiTableColumnFlags_WidthStretch);
				// CollapsingHeader overflows as half of window padding
				ImGui::TableSetupColumn(nullptr, ImGuiTableColumnFlags_WidthFixed, ImGui::GetStyle().WindowPadding.x * 0.5f); 
				ImGui::TableSetupColumn(nullptr, ImGuiTableColumnFlags_WidthFixed);
				ImGui::TableNextRow();
				
				ImGui::TableSetColumnIndex(0);
				opened = ImGui::CollapsingHeader(safeSceneName, ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanAvailWidth);

				ImGui::TableSetColumnIndex(2);
				const auto frameHeight = ImGui::GetFrameHeight();

				if (ImGui::Button("+", { frameHeight, frameHeight }))
					Entity::create(scene.get())->setName("New Entity");

				ImGui::EndTable();
			}

			if (opened) {
				for (const auto entity : scene->rootEntities())
					drawEntityNode(entity);
			}
		}
	}
}
