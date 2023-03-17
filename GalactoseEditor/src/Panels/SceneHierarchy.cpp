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
			| ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;

		if (children.empty())
			flags |= ImGuiTreeNodeFlags_Leaf;

		if (m_sceneData->selectedEntity() == a_entity)
			flags |= ImGuiTreeNodeFlags_Selected;
		
		const bool opened = ImGui::TreeNodeEx(a_entity, flags, a_entity->name().c_str());

		if (ImGui::IsItemClicked())
			m_sceneData->setSelectedEntity(a_entity);

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
				if (ImGui::Button("+"))
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
