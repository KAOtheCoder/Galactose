#include "Inspector.h"

#include <Scene/Components/Transform.h>

#include <imgui.h>

using namespace Galactose;

namespace GalactoseEditor {
	template <>
	void Inspector::drawComponent<Transform>() {
		if (!drawComponentHeader("Transform"))
			return;

		auto transform = getSelectedComponent<Transform>();

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
}
