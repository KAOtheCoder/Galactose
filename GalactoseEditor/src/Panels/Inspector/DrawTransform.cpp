#include "Inspector.h"
#include "Widgets/PropertyTable.h"

#include <Galactose/Scene/Components/Transform.h>

using namespace Galactose;

namespace GalactoseEditor {
	template <>
	void Inspector::drawComponentContent<Transform>(Transform* a_transform) {
		auto position = a_transform->localPosition();
		if (PropertyTable::dragVector("Position", 3, position.data(), true))
			a_transform->setLocalPosition(position);

		const auto& rotation = a_transform->localRotation();
		auto eulerAngles = rotation.fuzzyCompare(Quaternion::fromEulerDegrees(m_eulerAngles)) ? m_eulerAngles : rotation.eulerDegrees();

		if (PropertyTable::dragVector("Rotation", 3, eulerAngles.data(), true)) {
			a_transform->setLocalRotation(Quaternion::fromEulerDegrees(eulerAngles));
			m_eulerAngles = eulerAngles;
		}

		auto scale = a_transform->localScale();
		if (PropertyTable::dragVector("Scale", 3, scale.data(), true))
			a_transform->setLocalScale(scale);
	}
}
