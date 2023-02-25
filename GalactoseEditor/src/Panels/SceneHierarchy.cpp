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
			for (const auto entity : scene->rootEntites())
				drawEntityNode(entity);
		}
	}
}
