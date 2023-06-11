#include "Inspector.h"

#include <Galactose/Scene/Components/Camera.h>

#include <imgui.h>

using namespace Galactose;

namespace GalactoseEditor {
	template <>
	void Inspector::drawComponentContent<Camera>(Camera* a_camera) {
		drawLabel("Field of View");
		ImGui::TableSetColumnIndex(1);
		ImGui::PushItemWidth(-std::numeric_limits<float>().min());
		auto fov = a_camera->fov();
		if (ImGui::SliderFloat("##Field of View", &fov, 1, 179, "%.3f", ImGuiSliderFlags_AlwaysClamp))
			a_camera->setFOV(fov);
		ImGui::PopItemWidth();

		auto near = a_camera->nearClip();
		auto far = a_camera->farClip();
		const float EPSILON = 0.001f;

		if (dragFloat("Near Clip", near, 0.1f, EPSILON, far - EPSILON))
			a_camera->setNearClip(near);

		if (dragFloat("Far Clip", far, 0.1f, near + EPSILON, std::numeric_limits<float>().max()))
			a_camera->setFarClip(far);

		Vector4 color(a_camera->backgroundColor(), 1);
		if (colorButton("Background Color", color))
			a_camera->setBackgroundColor(color);
	}
}
