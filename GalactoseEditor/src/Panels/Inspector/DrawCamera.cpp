#include "Inspector.h"

#include <Scene/Components/Camera.h>

#include <imgui.h>

using namespace Galactose;

namespace GalactoseEditor {
	template <>
	void Inspector::drawComponentContent<Camera>() {
		auto camera = getSelectedComponent<Camera>();

		drawLabel("Field of View");
		ImGui::TableSetColumnIndex(1);
		ImGui::PushItemWidth(-std::numeric_limits<float>().min());
		auto fov = camera->fov();
		if (ImGui::SliderFloat("##Field of View", &fov, 1, 179, "%.3f", ImGuiSliderFlags_AlwaysClamp))
			camera->setFOV(fov);
		ImGui::PopItemWidth();

		auto near = camera->nearClip();
		auto far = camera->farClip();
		const float EPSILON = 0.001f;

		if (dragFloat("Near Clip", near, 0.1f, EPSILON, far - EPSILON))
			camera->setNearClip(near);

		if (dragFloat("Far Clip", far, 0.1f, near + EPSILON, std::numeric_limits<float>().max()))
			camera->setFarClip(far);
	}
}
