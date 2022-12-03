#include "Inspector.h"
#include "EditorSceneData.h"
#include "InputString.h"

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
		
		if (InputString::inputText("##Name", entity->name()))
			entity->setName(InputString::text());

		ImGui::Separator();
	}
}
