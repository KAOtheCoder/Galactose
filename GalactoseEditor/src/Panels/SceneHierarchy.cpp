#include "SceneHierarchy.h"
#include "EditorContext.h"
#include "Widgets/TrailingCollapsingHeader.h"

#include <Galactose/Core/Events/KeyEvent.h>

#include <imgui.h>

using namespace Galactose;

namespace GalactoseEditor {
	SceneHierarchy::SceneHierarchy(const std::shared_ptr<EditorContext>& a_sceneContext) :
		Panel("Scene Hierarchy"),
		m_sceneContext(a_sceneContext)
	{}

	void SceneHierarchy::drawEntityNode(Entity* a_entity) {
		const auto& children = a_entity->children();
		
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_OpenOnArrow 
			| ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;

		if (children.empty())
			flags |= ImGuiTreeNodeFlags_Leaf;

		bool selected = m_sceneContext->selectedEntity() == a_entity;
		
		if (selected)
			flags |= ImGuiTreeNodeFlags_Selected;
		
		bool opened = ImGui::TreeNodeEx(a_entity, flags, a_entity->name().c_str());

		if (ImGui::IsItemClicked()) {
			m_sceneContext->setSelectedEntity(a_entity);
			selected = true;
		}
		
		if (ImGui::IsItemHovered() || selected) {
			const auto frameHeight = ImGui::GetFrameHeight();
			ImGui::SameLine(ImGui::GetWindowWidth() - ImGui::GetStyle().WindowPadding.x - frameHeight);
			
			if (ImGui::Button("-", { frameHeight, frameHeight })) {
				a_entity->destroy();

				if (selected)
					m_sceneContext->setSelectedEntity(nullptr);

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
		const auto& scene = m_sceneContext->scene();
		if (scene) {
			if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsWindowHovered())
				m_sceneContext->setSelectedEntity(nullptr);

			const auto& sceneName = scene->name();
			const auto safeSceneName = sceneName.empty() ? "Untitled" : sceneName.c_str();
			bool createEntity = false;
			const bool opened = TrailingCollapsingHeader::draw(safeSceneName, "+", createEntity);

			if (createEntity)
				Entity::create(scene.get())->setName("New Entity");

			if (opened) {
				for (const auto entity : scene->rootEntities())
					drawEntityNode(entity);
			}
		}
	}

	void SceneHierarchy::onEvent(const std::shared_ptr<Event>& a_event) {
		if (a_event->type() == Event::KeyPress) {
			auto keyEvent = static_cast<KeyPressEvent*>(a_event.get());
			auto selectedEntity = m_sceneContext->selectedEntity();

			if (keyEvent->key() == KeyEvent::KeyDelete && !keyEvent->isRepeat() && selectedEntity) {
				selectedEntity->destroy();
				m_sceneContext->setSelectedEntity(nullptr);
				a_event->setHandled();
			}
		}
	}
}
