#include "Inspector.h"
#include "Widgets/PropertyTable.h"

#include <Galactose/Scene/Components/Camera.h>

using namespace Galactose;

namespace GalactoseEditor {
	template <>
	void Inspector::drawComponentContent<Camera>(Camera* a_camera) {
		auto fov = a_camera->fov();
		if (PropertyTable::sliderFloat("Field of View", fov, 1, 179))
			a_camera->setFOV(fov);

		auto near = a_camera->nearClip();
		auto far = a_camera->farClip();
		const float EPSILON = 0.001f;

		if (PropertyTable::dragFloat("Near Clip", near, 0.1f, EPSILON, far - EPSILON))
			a_camera->setNearClip(near);

		if (PropertyTable::dragFloat("Far Clip", far, 0.1f, near + EPSILON, std::numeric_limits<float>().max()))
			a_camera->setFarClip(far);

		Vector4 color(a_camera->backgroundColor(), 1);
		if (PropertyTable::colorButton("Background Color", color))
			a_camera->setBackgroundColor(color);
	}
}
