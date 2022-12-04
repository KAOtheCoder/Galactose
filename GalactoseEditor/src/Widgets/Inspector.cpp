#include "Inspector.h"
#include "EditorSceneData.h"
#include "InputString.h"

#include <Scene/Components/Transform.h>

#include <imgui.h>

using namespace Galactose;

namespace GalactoseEditor {
	Inspector::Inspector(const std::shared_ptr<EditorSceneData>& a_sceneData) :
		Widget("Inspector"),
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

	bool Inspector::dragVector3(const char* a_label, Galactose::Vector3& a_value) {
		return ImGui::DragFloat3(a_label, a_value.data());
	}

	void Inspector::drawTransform() {
		auto transform = m_sceneData->selectedEntity()->getTransform();

		auto position = transform->position();
		if (dragVector3("Position", position))
			transform->setPosition(position);

		auto rotation = transform->rotation().eulerDegrees();
		if (dragVector3("Rotation", rotation))
			transform->setRotation(Quaternion::fromEulerDegrees(rotation));

		auto scale = transform->scale();
		if (dragVector3("Scale", scale))
			transform->setScale(scale);
	}
}
