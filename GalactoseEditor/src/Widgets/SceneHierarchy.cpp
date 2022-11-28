#include "SceneHierarchy.h"

#include <Scene/Entity.h>

#include <imgui.h>

using namespace Galactose;

namespace GalactoseEditor {
	SceneHierarchy::SceneHierarchy(const std::shared_ptr<Scene>& a_scene) :
		m_scene(a_scene)
	{}

	void SceneHierarchy::drawEntityNode(const Entity* a_entity) {
		const auto& children = a_entity->getChildren();
		
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

		if (children.empty())
			flags |= ImGuiTreeNodeFlags_Leaf;
		
		if (ImGui::TreeNodeEx(a_entity, flags, a_entity->name().c_str())) {
			for (const auto child : children)
				drawEntityNode(child);
			
			ImGui::TreePop();
		}
	}

	void SceneHierarchy::onUpdate() {
		if (!m_visible)
			return;

		ImGui::Begin("Scene Hierarchy", &m_visible);
		
		const auto& scene = m_scene.lock();
		if (scene) {
			for (const auto entity : scene->getRootEntites())
				drawEntityNode(entity);
		}

		ImGui::End();
	}
}
