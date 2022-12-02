#include "Inspector.h"
#include "EditorSceneData.h"

#include <imgui.h>

namespace GalactoseEditor {
	Inspector::Inspector(const std::shared_ptr<EditorSceneData>& a_sceneData) :
		Widget("Inspector"),
		m_sceneData(a_sceneData)
	{}

	void Inspector::onUpdate() {
		auto entity = m_sceneData->selectedEntity();

		if (!entity)
			return;
		
		ImGui::Text(entity->name().c_str());
		ImGui::Separator();
	}
}
