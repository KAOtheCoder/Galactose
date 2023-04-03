#include "Inspector.h"

#include <Scene/Components/Transform.h>

#include <imgui.h>

using namespace Galactose;

namespace GalactoseEditor {
	template <>
	void Inspector::drawComponentContent<Transform>(Transform* a_transform) {
		auto position = a_transform->localPosition();
		if (dragVector("Position", 3, position.data()))
			a_transform->setLocalPosition(position);

		auto rotation = a_transform->localRotation().eulerDegrees();
		if (dragVector("Rotation", 3, rotation.data()))
			a_transform->setLocalRotation(Quaternion::fromEulerDegrees(rotation));

		auto scale = a_transform->localScale();
		if (dragVector("Scale", 3, scale.data()))
			a_transform->setLocalScale(scale);
	}
}
