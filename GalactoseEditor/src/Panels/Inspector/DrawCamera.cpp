#include "Inspector.h"

#include <Scene/Components/Camera.h>

#include <imgui.h>

using namespace Galactose;

namespace GalactoseEditor {
	template <>
	void Inspector::drawComponent<Camera>() {
		if (!drawComponentHeader("Camera"))
			return;

		auto camera = getSelectedComponent<Camera>();

		if (ImGui::BeginTable("Camera", 2, ImGuiTableFlags_SizingStretchProp)) {
			drawLabel("Field of View");
			ImGui::TableSetColumnIndex(1);
			auto fov = camera->fov();
			if (ImGui::SliderFloat("##Field of View", &fov, 45, 90))
				camera->setFOV(fov);

			drawLabel("Near Clip");
			ImGui::TableSetColumnIndex(1);
			auto near = camera->nearClip();
			if (ImGui::DragFloat("##Near Clip", &near))
				camera->setNearClip(near);

			drawLabel("Far Clip");
			ImGui::TableSetColumnIndex(1);
			auto far = camera->nearClip();
			if (ImGui::DragFloat("##Far Clip", &far))
				camera->setNearClip(far);

			ImGui::EndTable();
		}
	}
}
